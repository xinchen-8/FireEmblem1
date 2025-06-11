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

    std::string content = "  HIT\n  CRT\nAVOID\n";

    attackedHpPoint->m_Transform.scale = {0.8, 1.0};
    attackedHpPoint->m_Transform.translation = {130, -50};
    attackedHpPoint->SetVisible(false);
    attackedHpPoint->SetZIndex(11);

    auto aDLD = std::make_shared<Util::Text>(FONTPATH, FONT_SIZE, content, Util::Color(255, 255, 255), false);
    attackedDetailList->SetDrawable(aDLD);
    attackedDetailList->SetPivot({-aDLD->GetSize().x / 2, 0});
    attackedDetailList->m_Transform.translation = {130, -250};
    attackedDetailList->SetVisible(false);
    attackedDetailList->SetZIndex(11);

    attackedDetailPoint->m_Transform.scale = {0.3, 1.0};
    attackedDetailPoint->m_Transform.translation = {230, -250};
    attackedDetailPoint->SetVisible(false);
    attackedDetailPoint->SetZIndex(11);

    attackerHpPoint->m_Transform.scale = {0.8, 1.0};
    attackerHpPoint->m_Transform.translation = {-450, -50};
    attackerHpPoint->SetVisible(false);
    attackerHpPoint->SetZIndex(11);

    auto rDLD = std::make_shared<Util::Text>(FONTPATH, FONT_SIZE, content, Util::Color(255, 255, 255), false);
    attackerDetailList->SetDrawable(rDLD);
    attackerDetailList->SetPivot({-rDLD->GetSize().x / 2, 0});

    attackerDetailList->m_Transform.translation = {-450, -250};
    attackerDetailList->SetVisible(false);
    attackerDetailList->SetZIndex(11);

    attackerDetailPoint->m_Transform.scale = {0.3, 1.0};
    attackerDetailPoint->m_Transform.translation = {-350, -250};
    attackerDetailPoint->SetVisible(false);
    attackerDetailPoint->SetZIndex(11);
}

void BattleUI::load(std::shared_ptr<Character> attacker, std::shared_ptr<Character> attacked) {
    attackerCharacter = attacker;
    attackedCharacter = attacked;
    refreshHpPoint();

    std::shared_ptr<HandHeldItem> atk_hhi = attacker->getCurrentHandHeldItem();
    std::shared_ptr<HandHeldItem> atd_hhi = attacked->getCurrentHandHeldItem();
    // std::shared_ptr<Weapon> atk_weapon = std::dynamic_pointer_cast<Weapon>(attacker->getCurrentHandHeldItem());
    std::string content = "";
    int num = atk_hhi->getHit();
    for (int i = 0; i < 20; i++)
        content += (i < num / 5) ? "■" : "□";
    content += "\n";

    // (user->getSkl() + user->getLck()) / 2 + crt;
    num = (attacker->getSkl() + attacker->getLck()) / 2 + atk_hhi->getCrt();
    for (int i = 0; i < 20; i++)
        content += (i < num / 5) ? "■" : "□";
    content += "\n";

    num = attacker->getSpd() + attacker->getAvoid(); // acc - (Avoid + Spd)
    for (int i = 0; i < 20; i++)
        content += (i < num / 5) ? "■" : "□";
    content += "\n";

    std::string content2 = "";
    int num2 = (atd_hhi) ? atd_hhi->getHit() : 0;
    for (int i = 0; i < 20; i++)
        content2 += (i < num2 / 5) ? "■" : "□";
    content2 += "\n";

    num2 = (attacked->getSkl() + attacked->getLck()) / 2;
    num2 += (atd_hhi) ? atd_hhi->getCrt() : 0;
    for (int i = 0; i < 20; i++)
        content2 += (i < num2 / 5) ? "■" : "□";
    content2 += "\n";

    num2 = attacked->getSpd() + attacked->getAvoid();
    for (int i = 0; i < 20; i++)
        content2 += (i < num2 / 5) ? "■" : "□";
    content2 += "\n";

    auto rDPD = std::make_shared<Util::Text>(FONTPATH, FONT_SIZE, content, Util::Color(0, 255, 0), false);
    attackerDetailPoint->SetDrawable(rDPD);
    attackerDetailPoint->SetPivot({-rDPD->GetSize().x / 2, 0});

    auto dDPD = std::make_shared<Util::Text>(FONTPATH, FONT_SIZE, content2, Util::Color(0, 255, 0), false);
    attackedDetailPoint->SetDrawable(dDPD);
    attackedDetailPoint->SetPivot({-dDPD->GetSize().x / 2, 0});

    canCounterAttack = true;
    canFollowUpAttack = true;

    attackedCharacter->findHandHeldScope();
    auto range = attackedCharacter->getAttackRange();
    if (range.find(attacker->getAbsolutePos()) == range.end()) {
        LOG_INFO("Attacked Character's attack range doesn't contain Attacker's position!");
        canCounterAttack = false;
    }

    if (!atk_hhi) {
        LOG_ERROR("Attacker doesn't use Weapon!");
        return;
    }

    std::string reg = "";
    if (!atd_hhi) {
        LOG_INFO("Attacked isn't hold a weapon.");
        canCounterAttack = false;
        reg = "Default";
    } else
        reg = atd_hhi->getClassName();

    attackerImg = attacker->getClassName() + "_" + atk_hhi->getClassName();
    attackedImg = attacked->getClassName() + "_" + reg;

    std::shared_ptr<Util::Image> attackerD = (!attackerCharacter->isEnemy())
                                                 ? std::make_shared<Util::Image>(BATTLE_PLAYER + attackerImg + ".png")
                                                 : std::make_shared<Util::Image>(BATTLE_ENEMY + attackerImg + ".png");
    attackerGO->SetDrawable(attackerD);

    std::shared_ptr<Util::Image> attackedD = (!attackedCharacter->isEnemy())
                                                 ? std::make_shared<Util::Image>(BATTLE_PLAYER + attackedImg + ".png")
                                                 : std::make_shared<Util::Image>(BATTLE_ENEMY + attackedImg + ".png");
    attackedGO->SetDrawable(attackedD);

    int atkrSpd = attackerCharacter->getSpd();
    int atkdSpd = attackedCharacter->getSpd();
    int atkrW = attackerCharacter->getStr() - attackerCharacter->getCurrentHandHeldItem()->getMt();
    int atkdW = attackerCharacter->getStr() - attackerCharacter->getCurrentHandHeldItem()->getMt();
    atkrSpd -= (atkrW > 0) ? atkrW : 0;
    atkdSpd -= (atkdW > 0) ? atkdW : 0;

    if (atkrSpd > atkdSpd)
        followUpType = followType::Attacker;
    else if (canCounterAttack && (atkdSpd > atkrSpd))
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

    m_Transform.translation = {-150, 200};
    m_Transform.scale = {1.5, 1.5};
    setVisible(true);
}

void BattleUI::update() {
    if (!m_Visible)
        return;
    static int frame = 0;
    static bool isFinish = false;

    glm::ivec2 ATKDPosition = glm::ivec2(400, 0);
    glm::ivec2 ATKRPosition = glm::ivec2(-400, 0);
    glm::ivec2 Step = glm::ivec2(5, 0);

    if (attackedCharacter->isEnemy() != attackerCharacter->isEnemy()) {
        if (isFinish && (frame == 57 || frame == 113 || frame == 123 || frame == 181)) {
            attackerCharacter->freshItem();
            attackedCharacter->freshItem();
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
            attackerGO->m_Transform.translation += Step;
            break;

        case 25: {
            int regHP = attackedCharacter->getCurHP();
            if (attackerCharacter->attack(attackedCharacter)) {
                attackedGO->SetDrawable(
                    (!attackedCharacter->isEnemy())
                        ? std::make_shared<Util::Image>(BATTLE_PLAYER "Attacked_" + attackedImg + ".png")
                        : std::make_shared<Util::Image>(BATTLE_ENEMY "Attacked_" + attackedImg + ".png"));
                regHP -= attackedCharacter->getCurHP();
                refreshHpPoint();
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
            if (attackedCharacter->getCurHP() == 0) {
                setString(attackedCharacter->getName() + " was defeated!");
                LOG_INFO(attackedCharacter->getName() + " was defeated!");
                if (attackedCharacter->isEnemy() && !attackerCharacter->isEnemy()) {
                    attackerCharacter->addExp(attackedCharacter->getExp());
                }
            } else {
                attackedGO->m_Transform.translation = ATKDPosition;
                attackedGO->SetDrawable((!attackedCharacter->isEnemy())
                                            ? std::make_shared<Util::Image>(BATTLE_PLAYER + attackedImg + ".png")
                                            : std::make_shared<Util::Image>(BATTLE_ENEMY + attackedImg + ".png"));
            }
            break;

        case 45:
            if (attackerCharacter->getCurrentHandHeldItem()->getUses() == 0) {
                setString(attackerCharacter->getName() + "'s weapon broke!");
                LOG_INFO(attackerCharacter->getName() + "'s weapon broke!");
            }
        case 46:
        case 47:
        case 48:
            attackerGO->m_Transform.translation -= Step;
            break;

        case 49:
            if (attackedCharacter->getCurHP() == 0)
                isFinish = true;
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
            attackedGO->m_Transform.translation -= Step;
            break;

        case 81: {
            int regHP = attackerCharacter->getCurHP();
            if (attackedCharacter->attack(attackerCharacter)) {
                attackerGO->SetDrawable(
                    (!attackerCharacter->isEnemy())
                        ? std::make_shared<Util::Image>(BATTLE_PLAYER "Attacked_" + attackerImg + ".png")
                        : std::make_shared<Util::Image>(BATTLE_ENEMY "Attacked_" + attackerImg + ".png"));
                regHP -= attackerCharacter->getCurHP();
                refreshHpPoint();
                setString(std::to_string(regHP) + " damage!");
                LOG_INFO(std::to_string(regHP) + " damage!");
            } else {
                attackerGO->m_Transform.translation += Step;
                setString(attackerCharacter->getName() + " dodges!");
                LOG_INFO(attackerCharacter->getName() + " dodges!");
            }
            break;
        }

        case 91:
            if (attackerCharacter->getCurHP() == 0) {
                setString(attackerCharacter->getName() + " was defeated!");
                LOG_INFO(attackerCharacter->getName() + " was defeated!");
                if (!attackedCharacter->isEnemy() && attackerCharacter->isEnemy()) {
                    attackedCharacter->addExp(attackerCharacter->getExp());
                }
            } else {
                attackerGO->m_Transform.translation = ATKRPosition;
                attackerGO->SetDrawable((!attackerCharacter->isEnemy())
                                            ? std::make_shared<Util::Image>(BATTLE_PLAYER + attackerImg + ".png")
                                            : std::make_shared<Util::Image>(BATTLE_ENEMY + attackerImg + ".png"));
            }
            break;

        case 101:
            if (attackedCharacter->getCurrentHandHeldItem()->getUses() == 0) {
                setString(attackedCharacter->getName() + "'s weapon broke!");
                LOG_INFO(attackedCharacter->getName() + "'s weapon broke!");
            }
        case 102:
        case 103:
        case 104:
            attackedGO->m_Transform.translation += Step;
            break;

        // follow up
        case 114:
            if (attackerCharacter->getCurHP() == 0 || attackedCharacter->getCurHP() == 0 ||
                (followUpType == followType::Attacked && attackedCharacter->getCurrentHandHeldItem() &&
                 attackedCharacter->getCurrentHandHeldItem()->getUses() == 0) ||
                !canFollowUpAttack)
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
                attackerGO->m_Transform.translation += Step;
            else
                attackedGO->m_Transform.translation -= Step;
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
                    refreshHpPoint();
                    setString(std::to_string(regHP) + " damage!");
                    LOG_INFO(std::to_string(regHP) + " damage!");
                } else {
                    attackedGO->m_Transform.translation += Step;
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
                    refreshHpPoint();
                    setString(std::to_string(regHP) + " damage!");
                    LOG_INFO(std::to_string(regHP) + " damage!");
                } else {
                    attackerGO->m_Transform.translation -= Step;
                    setString(attackerCharacter->getName() + " dodges!");
                    LOG_INFO(attackerCharacter->getName() + " dodges!");
                }
            }
            break;
        }

        case 157: {
            if (followUpType == followType::Attacker) {
                if (attackedCharacter->getCurHP() == 0) {
                    setString(attackedCharacter->getName() + " was defeated!");
                    LOG_INFO(attackedCharacter->getName() + " was defeated!");
                    if (attackedCharacter->isEnemy() && !attackerCharacter->isEnemy()) {
                        attackerCharacter->addExp(attackedCharacter->getExp());
                    }
                } else {
                    attackedGO->m_Transform.translation = glm::ivec2(400, 0);
                    attackedGO->SetDrawable((!attackedCharacter->isEnemy())
                                                ? std::make_shared<Util::Image>(BATTLE_PLAYER + attackedImg + ".png")
                                                : std::make_shared<Util::Image>(BATTLE_ENEMY + attackedImg + ".png"));
                }
            } else {
                if (attackerCharacter->getCurHP() == 0) {
                    setString(attackerCharacter->getName() + " was defeated!");
                    LOG_INFO(attackerCharacter->getName() + " was defeated!");
                    if (!attackedCharacter->isEnemy() && attackerCharacter->isEnemy()) {
                        attackedCharacter->addExp(attackerCharacter->getExp());
                    }
                } else {
                    attackerGO->m_Transform.translation = glm::ivec2(-400, 0);
                    attackerGO->SetDrawable((!attackerCharacter->isEnemy())
                                                ? std::make_shared<Util::Image>(BATTLE_PLAYER + attackerImg + ".png")
                                                : std::make_shared<Util::Image>(BATTLE_ENEMY + attackerImg + ".png"));
                }
            }
            break;
        }

        case 167:
            if (attackerCharacter->getCurrentHandHeldItem() &&
                attackerCharacter->getCurrentHandHeldItem()->getUses() == 0 && followUpType == followType::Attacker) {
                setString(attackerCharacter->getName() + "'s weapon broke!");
                LOG_INFO(attackerCharacter->getName() + "'s weapon broke!");
            } else if (attackedCharacter->getCurrentHandHeldItem() &&
                       attackedCharacter->getCurrentHandHeldItem()->getUses() == 0) {
                setString(attackedCharacter->getName() + "'s weapon broke!");
                LOG_INFO(attackedCharacter->getName() + "'s weapon broke!");
            }

        case 168:
        case 169:
        case 170:
            if (followUpType == followType::Attacker)
                attackerGO->m_Transform.translation -= Step;
            else
                attackedGO->m_Transform.translation += Step;
            break;

        case 180:
            isFinish = true;
            break;
        default:
            break;
        }
    } else {
        if (frame == 35) {
            attackerCharacter->freshItem();
            isFinish = false;
            finishTrigger = true;
            setVisible(false);
            frame = 0;
            return;
        }

        frame++;
        switch (frame) {
        case 2:
            setString(attackerCharacter->getName() + " uses Heal!");
            LOG_INFO(attackerCharacter->getName() + " uses Heal!");
            break;
        case 12:
        case 13:
        case 14:
        case 15:
            attackerGO->m_Transform.translation += Step;
            break;

        case 25: {
            int regHP = attackedCharacter->getCurHP();
            if (attackerCharacter->attack(attackedCharacter)) {
                regHP = attackedCharacter->getCurHP() - regHP;
                refreshHpPoint();
                setString(attackedCharacter->getName() + "'s HP is restored by " + std::to_string(regHP) + ".");
                LOG_INFO(attackedCharacter->getName() + "'s HP is restored by " + std::to_string(regHP) + ".");
            }
        }
        }
    }
}
void BattleUI::refreshHpPoint() {
    std::string hpC = "";
    int hpLimit = attackerCharacter->getHpLimit();
    int hpCurrent = attackerCharacter->getCurHP();
    for (int i = 0; i < hpLimit; i++) {
        hpC += (i < hpCurrent) ? "■" : "□";
        if (i % 10 == 0 && i != 0)
            hpC += "\n";
    }
    auto HPPD = std::make_shared<Util::Text>(FONTPATH, FONT_SIZE, hpC, Util::Color(0, 50, 255), false);
    attackerHpPoint->SetDrawable(HPPD);
    attackerHpPoint->SetPivot({-HPPD->GetSize().x / 2, 0});

    std::string hpC2 = "";
    int hpLimit2 = attackedCharacter->getHpLimit();
    int hpCurrent2 = attackedCharacter->getCurHP();
    for (int i = 0; i < hpLimit2; i++) {
        hpC2 += (i < hpCurrent2) ? "■" : "□";
        if (i % 10 == 0 && i != 0)
            hpC2 += "\n";
    }
    auto HPPD2 = std::make_shared<Util::Text>(FONTPATH, FONT_SIZE, hpC2, Util::Color(0, 50, 255), false);
    attackedHpPoint->SetDrawable(HPPD2);
    attackedHpPoint->SetPivot({-HPPD2->GetSize().x / 2, 0});
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
    attackerHpPoint->SetVisible(visible);
    attackedHpPoint->SetVisible(visible);
    attackerDetailList->SetVisible(visible);
    attackerDetailPoint->SetVisible(visible);
    attackedDetailList->SetVisible(visible);
    attackedDetailPoint->SetVisible(visible);
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
    children.push_back(attackerHpPoint);
    children.push_back(attackedHpPoint);
    children.push_back(attackerDetailList);
    children.push_back(attackerDetailPoint);
    children.push_back(attackedDetailList);
    children.push_back(attackedDetailPoint);
    return children;
}

bool BattleUI::isFinish() {
    if (finishTrigger) {
        finishTrigger = false;
        return true;
    }
    return false;
}