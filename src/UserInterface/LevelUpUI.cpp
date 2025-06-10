#include "UserInterface/LevelUpUI.hpp"

LevelUpUI::LevelUpUI(std::vector<std::shared_ptr<Tile>>& tiles) : UserInterface(tiles) {
    setUISize({5, 5});
    setRelativePos({
        -floor(PTSD_Config::WINDOW_WIDTH / 2) + 9 * TILE_SIZE,
        -floor(PTSD_Config::WINDOW_HEIGHT / 2) + 5 * TILE_SIZE
    });
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
        character->getName() + " Level Up!\n" +
        std::to_string(character->getLevel()) + "  Exp " + std::to_string(character->getExp()) + "\n" + "HP  " +
        std::to_string(character->getCurHP()) + " / " + std::to_string(character->getHpLimit()) + "\n" + "Str " +
        std::to_string(character->getStr()) + "  Lck " + std::to_string(character->getLck()) + "\n" + "Skl " +
        std::to_string(character->getSkl()) + "  Def " + std::to_string(character->getDef()) + "\n" + "Wlv " +
        std::to_string(character->getWlv()) + "  Res " + std::to_string(character->getRes()) + "\n" + "Spd " +
        std::to_string(character->getSpd()) + "  Mov " + std::to_string(character->getMov());
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