#include "UserInterface/LevelUpUI.hpp"

LevelUpUI::LevelUpUI(std::vector<std::shared_ptr<Tile>>& tiles) : UserInterface(tiles) {
    setUISize({8, 6});
    setRelativePos({
        -floor(PTSD_Config::WINDOW_WIDTH / 2) + 1 * TILE_SIZE,
        -floor(PTSD_Config::WINDOW_HEIGHT / 2) + 3 * TILE_SIZE
    });
}

void LevelUpUI::load(std::shared_ptr<Character> character) {
    if (character) {
        this->character = character;
        m_DisplayTime = 0.0f;
        m_Visible = true;
    }
}

void LevelUpUI::update() {
    if (!m_Visible || !character) return;

    m_DisplayTime += 0.016f; // Assuming 60 FPS
    if (m_DisplayTime >= DISPLAY_DURATION) {
        m_Visible = false;
        character->resetLevelUpFlag();
        return;
    }

    std::string content = 
        character->getName() + " Level Up!\n" +
        "Level " + std::to_string(character->getLevel()) + "\n" +
        "HP: " + std::to_string(character->getHpLimit()) + "\n" +
        "Str: " + std::to_string(character->getStr()) + "\n" +
        "Skl: " + std::to_string(character->getSkl()) + "\n" +
        "Spd: " + std::to_string(character->getSpd()) + "\n" +
        "Lck: " + std::to_string(character->getLck()) + "\n" +
        "Def: " + std::to_string(character->getDef()) + "\n" +
        "Res: " + std::to_string(character->getRes());

    setString(content);
}

void LevelUpUI::setVisible(bool visible) {
    m_Visible = visible;
    if (!visible) {
        m_DisplayTime = 0.0f;
    }
}

std::vector<std::shared_ptr<GameObject>> LevelUpUI::getChildren() {
    return {};
} 