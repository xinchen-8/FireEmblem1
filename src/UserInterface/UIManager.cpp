#include "UserInterface/UIManager.hpp"

UIManager::UIManager(std::shared_ptr<Selection> s, std::shared_ptr<MapManager> tm, std::shared_ptr<PlayerManager> pm,
                     std::shared_ptr<EnemyManager> em)
    : mapManager(tm), selection(s), playerManager(pm), enemyManager(em) {

    for (int i = 0; i < 9; i++) {
        tiles.push_back(
            std::make_shared<Tile>("F" + std::to_string(i + 1), TILE_UI "text" + std::to_string(i) + ".png"));
        tiles[i]->SetZIndex(6);
    }

    loadLevel = std::make_shared<LoadUI>(tiles);
    tileInfo = std::make_shared<TileInfoUI>(tiles);
    // characterInfo = std::make_shared<CharacterInfoUI>(tiles);
    characterInfoFull = std::make_shared<CharacterInfoUIFull>(tiles);
    selectedAct = std::make_shared<ActUI>(tiles);
    selectedWeapon = std::make_shared<WeaponUI>(tiles);
    selectedItem = std::make_shared<ItemUI>(tiles);
    battle = std::make_shared<BattleUI>(tiles);

    loadLevel->load(mapManager->getLevel());
    load();
    tileInfo->setVisible(true);
    // characterInfo->setVisible(true);
    characterInfoFull->setVisible(true);
}

void UIManager::load() {
    tileInfo->load(mapManager->getPosTile(selection->getAbsolutePos()));

    auto s = selection->getSelectCharacter();
    auto c = playerManager->getPosLevelCharacter(selection->getAbsolutePos());
    auto e = enemyManager->getPosLevelCharacter(selection->getAbsolutePos());

    if (c) {
        // characterInfo->load(c);
        characterInfoFull->load(c);
    } else if (e) {
        // characterInfo->load(e);
        characterInfoFull->load(e);
    } else if (s && selection->getStatus() == SelectionStatus::Moving) {
        // characterInfo->load(s);
        characterInfoFull->load(s);
    }
}

void UIManager::update() {
    tileInfo->update();
    // characterInfo->update();
    characterInfoFull->update();
}

bool UIManager::closeLoadUI() {
    if (loadLevel->getVisible() && !loadLevel->getGameOver()) {
        loadLevel->setVisible(false);
        selection->setStatus(SelectionStatus::Normal);
        return true;
    }
    return false;
}

void UIManager::loadActUI() {
    // if(selectedAct->getVisible()) return;

    std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();
    std::vector<bool> flags = {};

    flags.push_back(selectedCharacter->getName() == "Marth" &&
                    mapManager->nextLevel(selectedCharacter->getAbsolutePos())); //"Next"

    flags.push_back((selectedCharacter->getName() == "Marth") && mapManager->getLevel() == 1 &&
                    !playerManager->getCharacter("Wrys")->getVisible() &&
                    playerManager->getCharacter("Wrys")->getAbsolutePos() - glm::ivec2{0, TILE_SIZE} ==
                        selectedCharacter->getAbsolutePos()); //"Visit"

    auto darros = playerManager->getCharacter("Darros");
    auto castor = playerManager->getCharacter("Castor");
    flags.push_back(
        (playerManager->isNearEnemy("Marth", "Darros") && darros->getCurHP() == darros->getHpLimit()) ||
        (playerManager->isNearEnemy("Caeda", "Castor") && castor->getCurHP() == castor->getHpLimit())); //"Talk"

    flags.push_back(selectedCharacter->getAttackRange().size() != 0); //"Attack"
    flags.push_back(true);                                            //"Item"
    flags.push_back(true);                                            //"Wait"
    //"Trade" not yet
    selectedAct->load(flags, (selectedCharacter->getCurrentHandHeldItem()->getName() == "Heal"));
    selectedAct->setVisible(true);
}

bool UIManager::activeActUI() {
    std::string act = selectedAct->getActive();
    std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();

    if (act == "Next") {
        selection->setStatus(SelectionStatus::loadUI);
        return loadLevel->load(mapManager->getLevel() + 1);

    } else if (act == "Attack" && selectedCharacter) {
        LOG_INFO("Select Attack Option");
        selection->setStatus(SelectionStatus::AttackTargeting);

        playerManager->buildCharacterTips(selectedCharacter);
        if (selectedCharacter->getAttackRange().size() == 0)
            LOG_ERROR("NO ENEMY...");
        selection->setMoveLimit(selectedCharacter->getAttackRange());
        selection->setAbsolutePos(selection->getLimitRange().begin()->first);

    } else if (act == "Target" && selectedCharacter) {
        LOG_INFO("Select Target Option");
        selection->setStatus(SelectionStatus::Targeting);

        playerManager->buildCharacterTips(selectedCharacter);
        if (selectedCharacter->getAttackRange().size() == 0)
            LOG_ERROR("NO PLAYER...");
        selection->setMoveLimit(selectedCharacter->getAttackRange());
        selection->setAbsolutePos(selection->getLimitRange().begin()->first);

    } else if (act == "Visit" && selectedCharacter) {
        LOG_INFO("Select Visit Option");
        playerManager->WryTrigger();
        selection->setStatus(SelectionStatus::Normal);

        selectedCharacter->setStatus(CharacterStatus::Waiting);
        playerManager->removeUnwaitingCharacter(selectedCharacter);
        playerManager->clearTips();

    } else if (act == "Talk" && selectedCharacter) {
        LOG_INFO("Select Talk Option");
        if (playerManager->isNearEnemy("Marth", "Darros"))
            playerManager->TalkTrigger("Darros");

        else if (playerManager->isNearEnemy("Caeda", "Castor"))
            playerManager->TalkTrigger("Castor");

        else
            LOG_ERROR("NO TALK TARGET...");

        selection->setStatus(SelectionStatus::Normal);
        selectedCharacter->setStatus(CharacterStatus::Waiting);
        playerManager->removeUnwaitingCharacter(selectedCharacter);
        playerManager->clearTips();

    } else if (act == "Item" && selectedCharacter) {
        LOG_INFO("Select Item Option");
        selection->setStatus(SelectionStatus::ITEMIUI);
        loadItemUI();
        selectedItem->setVisible(true);

    } else if (act == "Wait" && selectedCharacter) {
        LOG_INFO("Select Wait Option");
        selection->setStatus(SelectionStatus::Normal);
        selectedCharacter->setStatus(CharacterStatus::Waiting);
        playerManager->removeUnwaitingCharacter(selectedCharacter);
        playerManager->clearTips();

    } else
        LOG_ERROR("NO Act With: " + act);
    selectedAct->setVisible(false);
    return false;
}

void UIManager::loadWeaponUI(glm::ivec2 targetPos, bool isHealSpecialCase) {
    // if(selectedWeapon->getVisible()) return;

    std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();
    int steps = 0;
    glm::ivec2 distance = selectedCharacter->getAbsolutePos() - targetPos;
    steps += abs(distance.x) / TILE_SIZE;
    steps += abs(distance.y) / TILE_SIZE;

    std::vector<std::shared_ptr<Item>> items = selectedCharacter->getItems();
    std::vector<std::shared_ptr<HandHeldItem>> HHitems = {nullptr, nullptr, nullptr, nullptr};

    if (isHealSpecialCase) {
        for (int i = 0; i < items.size(); i++) {
            auto heal = std::dynamic_pointer_cast<Heal>(items[i]);
            if (!heal)
                HHitems[i] = nullptr;
            else
                HHitems[i] = std::dynamic_pointer_cast<HandHeldItem>(items[i]);
        }
    } else {
        for (int i = 0; i < items.size(); i++) {
            auto weapon = std::dynamic_pointer_cast<Weapon>(items[i]);
            if (!weapon)
                HHitems[i] = nullptr;
            else {
                for (auto &r : weapon->getRng()) {
                    if (r == steps) {
                        HHitems[i] = std::dynamic_pointer_cast<HandHeldItem>(items[i]);
                        break;
                    }
                }
            }
        }
    }
    selectedWeapon->loadWeapon(HHitems);
    selectedWeapon->setVisible(true);
}

void UIManager::actWeaponUI() {
    std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();
    selectedCharacter->setHandHeldItemWithIndex(selectedWeapon->getSelectPointIndex());
    selectedWeapon->setVisible(false);
}

void UIManager::loadItemUI() {
    std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();
    std::vector<std::shared_ptr<Item>> items = selectedCharacter->getItems();
    int index = selectedCharacter->getHandHeldIndex();

    selectedItem->loadItem(items, index);
    selectedItem->setVisible(true);
}

void UIManager::actItemUI() {
    std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();
    std::shared_ptr<Item> item = selectedCharacter->getItems()[selectedItem->getSelectPointIndex()];
    std::shared_ptr<HandHeldItem> hhi = std::dynamic_pointer_cast<HandHeldItem>(item);
    std::shared_ptr<Vulnerary> vi = std::dynamic_pointer_cast<Vulnerary>(item);

    if (hhi) {
        selectedCharacter->setHandHeldItemWithIndex(selectedItem->getSelectPointIndex());
        LOG_INFO("Now " + selectedCharacter->getName() + " Hand Holding the " + hhi->getName() + ". ");
    } else if (vi) {
        selectedCharacter->useVulnerary(selectedItem->getSelectPointIndex());
    } else
        LOG_ERROR("unkown Item Error!");
    selectedItem->setVisible(false);
}

void UIManager::loadBattleUI(std::shared_ptr<Character> attacker, std::shared_ptr<Character> attacked) {
    selection->setStatus(SelectionStatus::BattleUI);
    battle->load(attacker, attacked);
}

bool UIManager::updateBattleUI() {
    battle->update();

    if (battle->isFinish()) {
        std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();
        selectedCharacter->setStatus(CharacterStatus::Waiting);
        playerManager->removeUnwaitingCharacter(selectedCharacter);
        playerManager->clearTips();
        selection->setStatus(SelectionStatus::Normal);
        return true; // for accessInput
    }
    return !battle->getVisible();
}

void UIManager::changeVisibleTileInfo() { tileInfo->setVisible(!tileInfo->getVisible()); }

// void UIManager::changeVisibleCharacterInfo() {
// 	characterInfo->setVisible(!characterInfo->getVisible());
// }

void UIManager::changeVisibleCharacterInfoFull() { characterInfoFull->setVisible(!characterInfoFull->getVisible()); }

std::vector<std::shared_ptr<Util::GameObject>> UIManager::getChildren() {
    std::vector<std::shared_ptr<Util::GameObject>> children = {};
    std::vector<std::shared_ptr<Util::GameObject>> reg = tileInfo->getChildren();
    // std::vector<std::shared_ptr<Util::GameObject>> c = characterInfo->getChildren();
    std::vector<std::shared_ptr<Util::GameObject>> cf = characterInfoFull->getChildren();
    std::vector<std::shared_ptr<Util::GameObject>> a = selectedAct->getChildren();
    std::vector<std::shared_ptr<Util::GameObject>> w = selectedWeapon->getChildren();
    std::vector<std::shared_ptr<Util::GameObject>> i = selectedItem->getChildren();
    std::vector<std::shared_ptr<Util::GameObject>> b = battle->getChildren();
    std::vector<std::shared_ptr<Util::GameObject>> ll = loadLevel->getChildren();

    for (auto &e : reg)
        children.push_back(std::static_pointer_cast<Util::GameObject>(e));
    children.push_back(tileInfo);
    // for (auto &e : c) children.push_back(std::static_pointer_cast<Util::GameObject>(e));
    // children.push_back(characterInfo);
    for (auto &e : cf)
        children.push_back(std::static_pointer_cast<Util::GameObject>(e));
    children.push_back(characterInfoFull);
    for (auto &e : a)
        children.push_back(std::static_pointer_cast<Util::GameObject>(e));
    children.push_back(selectedAct);
    for (auto &e : w)
        children.push_back(std::static_pointer_cast<Util::GameObject>(e));
    children.push_back(selectedWeapon);
    for (auto &e : i)
        children.push_back(std::static_pointer_cast<Util::GameObject>(e));
    children.push_back(selectedItem);
    for (auto &e : b)
        children.push_back(std::static_pointer_cast<Util::GameObject>(e));
    children.push_back(battle);
    for (auto &e : ll)
        children.push_back(std::static_pointer_cast<Util::GameObject>(e));
    children.push_back(loadLevel);
    return children;
}