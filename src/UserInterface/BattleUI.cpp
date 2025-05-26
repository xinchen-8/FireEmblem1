#include "UserInterface/BattleUI.hpp"
#include "config.hpp"

BattleUI::BattleUI(std::vector<std::shared_ptr<Tile>> &tiles) : UserInterface(tiles) {
    setUISize({floor(PTSD_Config::WINDOW_WIDTH / TILE_SIZE) + 2, floor(PTSD_Config::WINDOW_HEIGHT / TILE_SIZE) + 2});
    setRelativePos({-floor(PTSD_Config::WINDOW_WIDTH / 2), -floor(PTSD_Config::WINDOW_HEIGHT / 2)});

    for (auto &r : form) {
        for (auto &t : r) {
            t->SetZIndex(9);
        }
    }
    SetZIndex(10);
}

void BattleUI::load(std::shared_ptr<Character> attacker, std::shared_ptr<Character> attacked) {
    attackerCharacter = attacker;
    attackedCharacter = attacked;

    std::shared_ptr<Weapon> atk_weapon = std::dynamic_pointer_cast<Weapon>(attacker->getCurrentHandHeldItem());
    std::shared_ptr<Weapon> weapon = std::dynamic_pointer_cast<Weapon>(attacked->getCurrentHandHeldItem());

    canCounterAttack = true;
    canFollowUpAttack = true;

    // before update, need to find their attack scope
    auto range = attackedCharacter->getAttackRange();
    if (range.find(attacker->getAbsolutePos()) == range.end()) {
        LOG_INFO("Attacked Character's attack range doesn't contain Attacker's position!");
        canCounterAttack = false;
    }

    if (!atk_weapon) {
        LOG_ERROR("Attacker doesn't use Weapon!");
        return;
    }

    std::string reg = "";
    if (!weapon) {
        LOG_INFO("Attacked isn't hold a weapon.");
        canCounterAttack = false;
        reg = "Default";
    } else
        reg = weapon->getClassName();

    attackerImg = attacker->getClassName() + "_" + atk_weapon->getClassName();
    attackedImg = attacked->getClassName() + "_" + reg;

    std::shared_ptr<Util::Image> attackerD = (!attackerCharacter->isEnemy())
                                                 ? std::make_shared<Util::Image>(BATTLE_PLAYER + attackerImg + ".png")
                                                 : std::make_shared<Util::Image>(BATTLE_ENEMY + attackerImg + ".png");
    attackerGO->SetDrawable(attackerD);

    std::shared_ptr<Util::Image> attackedD = (!attackedCharacter->isEnemy())
                                                 ? std::make_shared<Util::Image>(BATTLE_PLAYER + attackedImg + ".png")
                                                 : std::make_shared<Util::Image>(BATTLE_ENEMY + attackedImg + ".png");
    attackedGO->SetDrawable(attackedD);

    if (attackerCharacter->getSpd() - attackedCharacter->getSpd() > 5)
        followUpType = followType::Attacker;
    else if (canCounterAttack && (attackedCharacter->getSpd() - attackerCharacter->getSpd() > 5))
        followUpType = followType::Attacked;
    else
        canFollowUpAttack = false;

    attackerGO->m_Transform.translation = glm::ivec2(-400, 0);
    attackerGO->m_Transform.scale = {-TILE_SCALE, TILE_SCALE};
    attackerGO->SetZIndex(10);
    attackerGO->SetPivot({0, -attackerD->GetSize().y / 2});

    attackedGO->m_Transform.translation = glm::ivec2(400, 0);
    attackedGO->m_Transform.scale = {TILE_SCALE, TILE_SCALE};
    attackedGO->SetZIndex(10);
    attackedGO->SetPivot({0, -attackedD->GetSize().y / 2});

    SetPivot({0, 0});
    m_Transform.translation = {-250, -250};
    m_Transform.scale = {1.5, 1.5};
    setVisible(true);
}

void BattleUI::update() {
    if (!m_Visible)
        return;
    static int frame = 0;
    static bool isFinish = false;

    if (isFinish && (frame == 57 || frame == 113 || frame == 123 || frame == 181)) {
        attackerCharacter = nullptr;
        attackedCharacter = nullptr;
        isFinish = false;
        finishTrigger = true;
        setVisible(false);
        frame = 0;
        return;
    }

    frame++;
    switch (frame) {
    // attack
    case 2:
        setString(attackerCharacter->getName() + " attacks!");
        LOG_INFO(attackerCharacter->getName() + " attacks!");
        break;
    case 12:
    case 13:
    case 14:
    case 15:
        attackerGO->m_Transform.translation += glm::ivec2(5, 0);
        break;

    case 25: {
        int regHP = attackedCharacter->getCurHP();
        if (attackerCharacter->attack(attackedCharacter)) {
            attackedGO->SetDrawable(
                (!attackedCharacter->isEnemy())
                    ? std::make_shared<Util::Image>(BATTLE_PLAYER "Attacked_" + attackedImg + ".png")
                    : std::make_shared<Util::Image>(BATTLE_ENEMY "Attacked_" + attackedImg + ".png"));
            regHP -= attackedCharacter->getCurHP();
            setString(std::to_string(regHP) + " damage!");
            LOG_INFO(std::to_string(regHP) + " damage!");
        } else {
            attackedGO->m_Transform.translation += glm::ivec2(15, 0);
            setString(attackedCharacter->getName() + " dodges!");
            LOG_INFO(attackedCharacter->getName() + " dodges!");
        }
        break;
    }

    case 35:
        attackedGO->m_Transform.translation = glm::ivec2(400, 0);
        attackedGO->SetDrawable((!attackedCharacter->isEnemy())
                                    ? std::make_shared<Util::Image>(BATTLE_PLAYER + attackedImg + ".png")
                                    : std::make_shared<Util::Image>(BATTLE_ENEMY + attackedImg + ".png"));
        if (attackedCharacter->getCurHP() == 0) {
            isFinish = true;
            setString(attackedCharacter->getName() + " was defeated!");
            LOG_INFO(attackedCharacter->getName() + " was defeated!");
        }
        break;
    case 45:
    case 46:
    case 47:
    case 48:
        attackerGO->m_Transform.translation -= glm::ivec2(5, 0);
        break;

    case 49:
        if (!canCounterAttack)
            frame = 105;
        break;

    // counterattack
    case 58:
        setString(attackedCharacter->getName() + " counterattacks!");
        LOG_INFO(attackedCharacter->getName() + " counterattacks!");
        break;
    case 68:
    case 69:
    case 70:
    case 71:
        attackedGO->m_Transform.translation -= glm::ivec2(5, 0);
        break;

    case 81: {
        int regHP = attackerCharacter->getCurHP();
        if (attackedCharacter->attack(attackerCharacter)) {
            attackerGO->SetDrawable(
                (!attackerCharacter->isEnemy())
                    ? std::make_shared<Util::Image>(BATTLE_PLAYER "Attacked_" + attackerImg + ".png")
                    : std::make_shared<Util::Image>(BATTLE_ENEMY "Attacked_" + attackerImg + ".png"));
            regHP -= attackerCharacter->getCurHP();
            setString(std::to_string(regHP) + " damage!");
            LOG_INFO(std::to_string(regHP) + " damage!");
        } else {
            attackerGO->m_Transform.translation -= glm::ivec2(-5, 0);
            setString(attackerCharacter->getName() + " dodges!");
            LOG_INFO(attackerCharacter->getName() + " dodges!");
        }
        break;
    }

    case 91:
        attackerGO->m_Transform.translation = glm::ivec2(-400, 0);
        attackerGO->SetDrawable((!attackerCharacter->isEnemy())
                                    ? std::make_shared<Util::Image>(BATTLE_PLAYER + attackerImg + ".png")
                                    : std::make_shared<Util::Image>(BATTLE_ENEMY + attackerImg + ".png"));
        if (attackerCharacter->getCurHP() == 0) {
            isFinish = true;
            setString(attackerCharacter->getName() + " was defeated!");
            LOG_INFO(attackerCharacter->getName() + " was defeated!");
        }
        break;

    case 101:
    case 102:
    case 103:
    case 104:
        attackedGO->m_Transform.translation += glm::ivec2(5, 0);
        break;

    // follow up
    case 114:
        if (!canFollowUpAttack)
            isFinish = true;
        break;

    case 124:
        setString((followUpType == followType::Attacker) ? attackerCharacter->getName() + " follows up!"
                                                         : attackedCharacter->getName() + " follows up!");
        LOG_INFO((followUpType == followType::Attacker) ? attackerCharacter->getName() + " follows up!"
                                                        : attackedCharacter->getName() + " follows up!");
        break;
    case 134:
    case 135:
    case 136:
    case 137:
        if (followUpType == followType::Attacker)
            attackerGO->m_Transform.translation += glm::ivec2(5, 0);
        else
            attackedGO->m_Transform.translation -= glm::ivec2(5, 0);
        break;

    case 147: {
        if (followUpType == followType::Attacker) {
            int regHP = attackedCharacter->getCurHP();
            if (attackerCharacter->attack(attackedCharacter)) {
                attackedGO->SetDrawable(
                    (!attackedCharacter->isEnemy())
                        ? std::make_shared<Util::Image>(BATTLE_PLAYER "Attacked_" + attackedImg + ".png")
                        : std::make_shared<Util::Image>(BATTLE_ENEMY "Attacked_" + attackedImg + ".png"));
                regHP -= attackedCharacter->getCurHP();
                setString(std::to_string(regHP) + " damage!");
                LOG_INFO(std::to_string(regHP) + " damage!");
            } else {
                attackedGO->m_Transform.translation += glm::ivec2(5, 0);
                setString(attackedCharacter->getName() + " dodges!");
                LOG_INFO(attackedCharacter->getName() + " dodges!");
            }
        } else {
            int regHP = attackerCharacter->getCurHP();
            if (attackedCharacter->attack(attackerCharacter)) {
                attackerGO->SetDrawable(
                    (!attackerCharacter->isEnemy())
                        ? std::make_shared<Util::Image>(BATTLE_PLAYER "Attacked_" + attackerImg + ".png")
                        : std::make_shared<Util::Image>(BATTLE_ENEMY "Attacked_" + attackerImg + ".png"));
                regHP -= attackerCharacter->getCurHP();
                setString(std::to_string(regHP) + " damage!");
                LOG_INFO(std::to_string(regHP) + " damage!");
            } else {
                attackerGO->m_Transform.translation -= glm::ivec2(5, 0);
                setString(attackerCharacter->getName() + " dodges!");
                LOG_INFO(attackerCharacter->getName() + " dodges!");
            }
        }
        break;
    }

    case 157: {
        if (followUpType == followType::Attacker) {
            attackedGO->m_Transform.translation = glm::ivec2(400, 0);
            attackedGO->SetDrawable((!attackedCharacter->isEnemy())
                                        ? std::make_shared<Util::Image>(BATTLE_PLAYER + attackedImg + ".png")
                                        : std::make_shared<Util::Image>(BATTLE_ENEMY + attackedImg + ".png"));
            if (attackedCharacter->getCurHP() == 0) {
                isFinish = true;
                setString(attackedCharacter->getName() + " was defeated!");
                LOG_INFO(attackedCharacter->getName() + " was defeated!");
            }
        } else {
            attackerGO->m_Transform.translation = glm::ivec2(-400, 0);
            attackerGO->SetDrawable((!attackerCharacter->isEnemy())
                                        ? std::make_shared<Util::Image>(BATTLE_PLAYER + attackerImg + ".png")
                                        : std::make_shared<Util::Image>(BATTLE_ENEMY + attackerImg + ".png"));
            if (attackerCharacter->getCurHP() == 0) {
                isFinish = true;
                setString(attackerCharacter->getName() + " was defeated!");
                LOG_INFO(attackerCharacter->getName() + " was defeated!");
            }
        }
        break;
    }

    case 167:
    case 168:
    case 169:
    case 170:
        if (followUpType == followType::Attacker)
            attackerGO->m_Transform.translation -= glm::ivec2(5, 0);
        else
            attackedGO->m_Transform.translation += glm::ivec2(5, 0);
        break;

    case 180:
        isFinish = true;
        break;
    default:
        break;
    }
}

void BattleUI::setVisible(bool visible) {
    m_Visible = visible;
    for (auto &row : form) {
        for (auto &e : row) {
            e->SetVisible(visible);
        }
    }
    attackerGO->SetVisible(visible);
    attackedGO->SetVisible(visible);
}

std::vector<std::shared_ptr<Util::GameObject>> BattleUI::getChildren() {
    std::vector<std::shared_ptr<Util::GameObject>> children;
    for (auto &row : form) {
        for (auto &tile : row) {
            children.push_back(tile);
        }
    }
    children.push_back(attackerGO);
    children.push_back(attackedGO);
    return children;
}

bool BattleUI::isFinish() {
    if (finishTrigger) {
        finishTrigger = false;
        return true;
    }
    return false;
}