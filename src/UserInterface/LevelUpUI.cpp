#include "UserInterface/LevelUpUI.hpp"

LevelUpUI::LevelUpUI(std::vector<std::shared_ptr<Tile>> &tiles) : UserInterface(tiles) {
    setUISize({5, 7.5});
    setRelativePos({-floor(PTSD_Config::WINDOW_WIDTH / 2) + 8.5 * TILE_SIZE,
                    -floor(PTSD_Config::WINDOW_HEIGHT / 2) + 2 * TILE_SIZE});
    SetZIndex(20);
}

void LevelUpUI::load(std::shared_ptr<Character> character) {
    if (character) {
        this->character = character;
        m_DisplayTime = 0.0f;
        m_Visible = true;
        setVisible(true);
    }
}

void LevelUpUI::update() {
    if (!m_Visible || !character) {
        LOG_INFO("LevelUpUI::update - not visible or no character");
        return;
    }

    m_DisplayTime += 1.0f / TARGET_FPS;

    if (m_DisplayTime >= DISPLAY_DURATION) {
        m_Visible = false;
        setVisible(false);
        character->resetLevelUpFlag();
        return;
    }

    std::string content =
        character->getName() + " Level Up!\n" + "Lv " + std::to_string(character->getLevel()) + "  Exp " +
        std::to_string(character->getExp()) + "\n" + "HP " + std::to_string(character->getOldHpLimit()) + " -> " +
        std::to_string(character->getHpLimit()) + "\n" + "Str " + std::to_string(character->getOldStr()) + " -> " +
        std::to_string(character->getStr()) + "\n" + "Lck " + std::to_string(character->getOldLck()) + " -> " +
        std::to_string(character->getLck()) + "\n" + "Skl " + std::to_string(character->getOldSkl()) + " -> " +
        std::to_string(character->getSkl()) + "\n" + "Def " + std::to_string(character->getOldDef()) + " -> " +
        std::to_string(character->getDef()) + "\n" + "Wlv " + std::to_string(character->getOldWlv()) + " -> " +
        std::to_string(character->getWlv()) + "\n" + "Res " + std::to_string(character->getOldRes()) + " -> " +
        std::to_string(character->getRes()) + "\n" + "Spd " + std::to_string(character->getOldSpd()) + " -> " +
        std::to_string(character->getSpd());
    setString(content);
}

void LevelUpUI::setVisible(bool visible) {
    m_Visible = visible;
    UserInterface::setVisible(visible);
    if (!visible) {
        m_DisplayTime = 0.0f;
    }
}

std::vector<std::shared_ptr<Util::GameObject>> LevelUpUI::getChildren() {
    auto children = UserInterface::getChildren();
    return children;
}