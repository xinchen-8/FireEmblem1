#include "ProcessController.hpp"

ProcessController::ProcessController(std::shared_ptr<MapManager> mapManager,
                                     std::shared_ptr<PlayerManager> playerManager,
                                     std::shared_ptr<EnemyManager> enemyManager, std::shared_ptr<Selection> selection,
                                     std::shared_ptr<UIManager> uiManager, std::shared_ptr<Camera> camera)
    : mapManager(mapManager), playerManager(playerManager), enemyManager(enemyManager), selection(selection),
      uiManager(uiManager), camera(camera) {}

bool ProcessController::ReturnCase() {
    if (uiManager->closeLoadUI())
        return false;

    std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();
    std::shared_ptr<Character> selectMovableCharacter =
        playerManager->getPosMovableCharacter(selection->getAbsolutePos());
    std::shared_ptr<Character> selectLevelCharacter = playerManager->getPosLevelCharacter(selection->getAbsolutePos());
    std::shared_ptr<Character> selectEnemy = enemyManager->getPosLevelCharacter(selection->getAbsolutePos());
    SelectionStatus status = selection->getStatus();

    // normal -> moving
    if (status == SelectionStatus::Normal && selectMovableCharacter)
        normalToMoving(selectMovableCharacter);
    // moving => SUI
    else if (status == SelectionStatus::Moving &&
             (!selectLevelCharacter || selectMovableCharacter == selectedCharacter))
        movingToSUI(selectedCharacter);
    // SUI
    else if (status == SelectionStatus::Walking && selectedCharacter->getWalkPath().empty())
        SUItoOption();
    // act
    else if (status == SelectionStatus::SUI)
        return uiManager->activeActUI();
    // targetong(to player) => HUI(WUI only heal), only have a special case with Curate
    else if (status == SelectionStatus::Targeting)
        TargetingToHUI(selectLevelCharacter->getAbsolutePos());
    // HUI(WUI only heal) => wait, only have a special case with Curate
    else if (status == SelectionStatus::HUI)
        HealToNormal(selectedCharacter, selectLevelCharacter);
    // attack targeting => WUI
    else if (status == SelectionStatus::AttackTargeting && selectEnemy)
        ATKTargetingToATKWUI(selectEnemy->getAbsolutePos());
    // WUI => attack then wait
    else if (status == SelectionStatus::AttackWUI)
        ATKToNormal(selectedCharacter, selectEnemy);
    // item UI => SUI
    else if (status == SelectionStatus::ITEMIUI)
        IUIToSUI(selectedCharacter);
    return false;
}

void ProcessController::SpaceCase() {
    SelectionStatus status = selection->getStatus();
    if (status == SelectionStatus::ITEMIUI)
        uiManager->actItemUI(false);
}

void ProcessController::BackCase() {}

void ProcessController::MovCase(glm::ivec2 mov) {
    if (selection->getStatus() == SelectionStatus::SUI)
        return;
    selection->moveDirectly(mov);
    uiManager->load();
    uiManager->update();
}

void ProcessController::MovCase(int listMov) {
    uiManager->updateActUI(listMov);
    uiManager->updateWeaponUI(listMov);
    uiManager->updateItemUI(listMov);
    // other ui...
}

void ProcessController::normalToMoving(std::shared_ptr<Character> &selectCharacter) {
    selection->setSelectCharacter(selectCharacter);
    selection->setStatus(SelectionStatus::Moving);
    selection->setMoveLimit(playerManager->selectCharacter(selectCharacter));
}

void ProcessController::movingToSUI(std::shared_ptr<Character> &selectedCharacter) {
    selection->setStatus(SelectionStatus::Walking);
    playerManager->clearTips();
    selectedCharacter->buildWalkPath(selection->getAbsolutePos()); // trigger findCharacterAttackTarget when finished
}

void ProcessController::SUItoOption() { uiManager->activeActUI(); }

void ProcessController::TargetingToHUI(glm::ivec2 targetPos) {
    selection->setStatus(SelectionStatus::HUI);
    uiManager->loadWeaponUI(targetPos, true);
}

void ProcessController::HealToNormal(std::shared_ptr<Character> &selectedCharacter,
                                     std::shared_ptr<Character> &selectPlayer) {

    uiManager->actWeaponUI();
    uiManager->loadBattleUI(selectedCharacter, selectPlayer);
    uiManager->load();
    uiManager->update();
}

void ProcessController::ATKTargetingToATKWUI(glm::ivec2 targetPos) {
    selection->setStatus(SelectionStatus::AttackWUI);
    uiManager->loadWeaponUI(targetPos, false);
}

void ProcessController::ATKToNormal(std::shared_ptr<Character> &selectedCharacter,
                                    std::shared_ptr<Character> &selectEnemy) {
    uiManager->actWeaponUI();
    uiManager->loadBattleUI(selectedCharacter, selectEnemy);
    uiManager->load();
    uiManager->update();
}

void ProcessController::IUIToSUI(std::shared_ptr<Character> &selectedCharacter) {
    uiManager->actItemUI(true);
    selection->setStatus(SelectionStatus::SUI);
    uiManager->loadActUI();
}

bool ProcessController::enemyTurn(bool accessInput) {
    if (!accessInput)
        return false;
    if (enemyManager->getCurrentUnwaitedCharacters().size() == 0) {
        enemyManager->reloadUnwaitingCharacter();
        camera->setTraceObject(nullptr);
        camera->resetCameraAbsolutePos();
        return true; // finished
    }
    static int frame = 0;
    static std::vector<std::shared_ptr<Character>> playerOfRange = {};
    static std::shared_ptr<Character> TargetPlayer = nullptr;
    frame++;

    std::shared_ptr<Character> c = enemyManager->getCurrentUnwaitedCharacters()[0];
    camera->setTraceObject(std::dynamic_pointer_cast<CameraGameObject>(c));
    camera->resetCameraAbsolutePos();

    switch (frame) {
    case 1: {
        // 1. if there have a player(players) in attack range or move range

        std::unordered_set<glm::ivec2> mask = playerManager->getCharacterPos();
        std::unordered_set<glm::ivec2> reg = mapManager->getAbsoluteCantMovPosition();
        for (auto pos = reg.begin(); pos != reg.end(); pos++) {
            if (*pos == c->getAbsolutePos())
                continue;
            mask.insert(*pos);
        }
        c->refreshMoveRange(mask);
        c->findAttackRange();

        auto mr = c->getMoveRange();
        auto ar = c->getAttackRange();
        playerOfRange.clear();

        for (auto &[pos, mov] : mr) {
            if (playerManager->getPosLevelCharacter(pos) != nullptr)
                playerOfRange.push_back(playerManager->getPosLevelCharacter(pos));
        }
        for (auto &[pos, mov] : ar) {
            if (playerManager->getPosLevelCharacter(pos) != nullptr)
                playerOfRange.push_back(playerManager->getPosLevelCharacter(pos));
        }
        break;
    }
    // 1-1  => Attack Marth or Attack the one with the lowest HP
    case 2: {
        for (auto &p : playerOfRange) {
            if (!TargetPlayer) {
                TargetPlayer = p;
            } else if (p->getName() == "Marth") {
                TargetPlayer = p;
                break;
            } else if (p->getCurHP() < TargetPlayer->getCurHP()) {
                TargetPlayer = p;
            }
        }
        playerOfRange.clear();
        break;
    }
    case 3: {
        if (TargetPlayer) {
            int atk_rng = c->getCurrentHandHeldItem()->getRng()[0];

            glm::ivec2 dirs[] = {{TILE_SIZE, 0}, {-TILE_SIZE, 0}, {0, TILE_SIZE}, {0, -TILE_SIZE}};
            std::unordered_map<glm::ivec2, int> moveRange;
            moveRange[TargetPlayer->getAbsolutePos()] = 0;
            std::unordered_map<glm::ivec2, int> attackRange;

            std::queue<std::pair<glm::ivec2, int>> q;
            std::unordered_set<glm::ivec2> visited;

            for (const auto &pos : moveRange) {
                for (const auto &dir : dirs) {
                    glm::ivec2 next = pos.first + dir;
                    if (mapManager->getPosTile(next))
                        q.push({next, atk_rng - 1});
                }
            }

            while (!q.empty()) {
                auto [pos, atk_left] = q.front();
                q.pop();

                if (atk_left < 0)
                    continue;
                if (moveRange.find(pos) != moveRange.end())
                    continue;
                if (visited.find(pos) != visited.end())
                    continue;

                visited.insert(pos);
                attackRange[pos] = atk_left;
                // std::cout << "RED(" << pos.x << ", " << pos.y << ")" << std::endl;

                for (const auto &dir : dirs) {
                    q.push({pos + dir, atk_left - 1});
                }
            }

            std::unordered_map<glm::ivec2, int> ar = c->getMoveRange();
            for (const auto &[pos, mov] : attackRange) {
                if (ar.find(pos) != ar.end() &&
                    (!enemyManager->getPosLevelCharacter(pos) || pos == c->getAbsolutePos())) {
                    c->buildWalkPath(pos);
                    c->setStatus(CharacterStatus::Moving);
                    break;
                }
            }
            if (c->getWalkPath().empty()) {
                LOG_INFO("EnemyManager::enemyTurn: No valid path found for attack.");
                TargetPlayer = nullptr;
            } else
                break;
        }

        // 2. else go marth
        auto bestPath = findBestPathToMarth(c);
        std::cout << "Best path size: " << bestPath.size() << std::endl;

        if (bestPath.empty())
            LOG_ERROR("EnemyManager::enemyTurn: No valid path found for moving to Marth.");
        else {
            glm::ivec2 target = c->getAbsolutePos();
            auto mr = c->getMoveRange();
            for (const auto &[pos, cost] : bestPath) {
                if (mr.find(pos) != mr.end() && pos != c->getAbsolutePos()) {
                    if (heuristic(pos, c->getAbsolutePos()) > heuristic(target, c->getAbsolutePos()) &&
                        !enemyManager->getPosLevelCharacter(pos))
                        target = pos;
                }
            }
            c->buildWalkPath(target);
            c->setStatus(CharacterStatus::Moving);
        }
        break;
    }
    case 54: {
        if (!c->getWalkPath().empty())
            frame = 4;
        break;
    }
    case 55: {
        if (TargetPlayer) {
            uiManager->loadBattleUI(c, TargetPlayer);
        }
        break;
    }
    case 56: {
        c->setStatus(CharacterStatus::Waiting);
        enemyManager->removeUnwaitingCharacter(c);
        TargetPlayer = nullptr;
        frame = 0;
        break;
    }
    }
    return false; // not finished
}

int ProcessController::heuristic(const glm::ivec2 &a, const glm::ivec2 &b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

// A* Algorithm with cost == -1 treated as obstacle
std::unordered_map<glm::ivec2, int> ProcessController::findBestPathToMarth(std::shared_ptr<Character> &enemy) {
    glm::ivec2 start = enemy->getAbsolutePos();
    glm::ivec2 target = playerManager->getCharacter("Marth")->getAbsolutePos();

    // Min-heap sorted by f = g + h
    std::priority_queue<A_Node, std::vector<A_Node>, std::greater<A_Node>> openList;
    std::unordered_map<glm::ivec2, int> pathCost;
    std::unordered_map<glm::ivec2, glm::ivec2> cameFrom;

    auto moveCost = enemy->getWalkCost();

    openList.push({start, 0, heuristic(start, target), heuristic(start, target)});
    pathCost[start] = 0;

    const glm::ivec2 directions[] = {{TILE_SIZE, 0}, {-TILE_SIZE, 0}, {0, TILE_SIZE}, {0, -TILE_SIZE}};
    while (!openList.empty()) {
        A_Node current = openList.top();
        openList.pop();

        if (current.pos == target)
            break;

        for (auto &dir : directions) {
            glm::ivec2 nextPos = current.pos + dir;
            auto tile = mapManager->getPosTile(nextPos);
            if (!tile)
                continue;

            auto it = moveCost.find(tile->getName());
            int cost;
            if (it != moveCost.end()) {
                cost = it->second;
            } else {
                auto def = moveCost.find("Default");
                if (def == moveCost.end())
                    continue;
                cost = def->second;
            }
            if (cost < 0)
                continue;

            int newG = current.gCost + cost;
            auto old = pathCost.find(nextPos);
            if (old == pathCost.end() || newG < old->second) {
                cameFrom[nextPos] = current.pos;
                pathCost[nextPos] = newG;
                int f = newG + heuristic(nextPos, target);
                openList.push({nextPos, newG, heuristic(nextPos, target), f});
            }
        }
    }

    std::unordered_map<glm::ivec2, int> optimalPath;
    if (pathCost.find(target) == pathCost.end())
        return {};

    glm::ivec2 cur = target;
    while (cur != start) {
        optimalPath[cur] = pathCost[cur];
        cur = cameFrom[cur];
    }
    optimalPath[start] = 0;
    return optimalPath;
}
