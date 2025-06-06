#include "UserInterface/CharacterInfoUI.hpp"
namespace fs = std::filesystem;

// CharacterInfoUI::CharacterInfoUI(std::vector<std::shared_ptr<Tile>>& tiles) :
// 	UserInterface(tiles) {
// 	setUISize({ 5, 3 });

// 	setRelativePos({
// 		+ floor(PTSD_Config::WINDOW_WIDTH / 2) - 5 * TILE_SIZE,
// 		- floor(PTSD_Config::WINDOW_HEIGHT / 2) + 1 * TILE_SIZE
// 	});
// }

// void CharacterInfoUI::load(std::shared_ptr<Character> character) {
// 	if (character) this->character = character;
// }

// void CharacterInfoUI::update() {
//     std::string content=
// 		character->getName() + "\n" +
// 		character->getClassName() + "\n" +
// 		"HP " + std::to_string(character->getCurHP()) + "/" + std::to_string(character->getHpLimit()) + "\n";
// //first getHP() will be replaced by getCurHP()
//     setString(content);
// }

CharacterInfoUIFull::CharacterInfoUIFull(std::vector<std::shared_ptr<Tile>> &tiles) : UserInterface(tiles) {
    setUISize({5, 8});

    setRelativePos({-floor(PTSD_Config::WINDOW_WIDTH / 2) + 1 * TILE_SIZE,
                    -floor(PTSD_Config::WINDOW_HEIGHT / 2) + 3 * TILE_SIZE});
    // profile = std::make_shared<ImageUI>();
    // profile->setZIndex(m_ZIndex + 1);
    profile = std::make_shared<Tile>(*tiles[4], glm::ivec2(0, 0));
    std::string profilePath = TILE_UI "text4.png";
    profile->m_Transform.translation = glm::vec2({-floor(PTSD_Config::WINDOW_WIDTH / 2) + 2 * TILE_SIZE,
                                                  -floor(PTSD_Config::WINDOW_HEIGHT / 2) + 9 * TILE_SIZE});
    // load a default profile image
    profile->SetZIndex(m_ZIndex + 1);
}

void CharacterInfoUIFull::load(std::shared_ptr<Character> character) {
    if (character)
        this->character = character;
}

void CharacterInfoUIFull::update() {
    std::string content =
        character->getName() + "\n" + character->getClassName() + "\n" + "Lv  " +
        std::to_string(character->getLevel()) + "  Exp " + std::to_string(character->getExp()) + "\n" + "HP  " +
        std::to_string(character->getCurHP()) + " / " + std::to_string(character->getHpLimit()) + "\n" + "Str " +
        std::to_string(character->getStr()) + "  Lck " + std::to_string(character->getLck()) + "\n" + "Skl " +
        std::to_string(character->getSkl()) + "  Def " + std::to_string(character->getDef()) + "\n" + "Wlv " +
        std::to_string(character->getWlv()) + "  Res " + std::to_string(character->getRes()) + "\n" + "Spd " +
        std::to_string(character->getSpd()) + "  Mov " + std::to_string(character->getMov());
    setString(content);
    // std::string profilePath = TILE_UI "text4.png";
    std::string profilePath = TILE_PROFILE + character->getName() + "_0.png";
    // acording to the character name, load the profile image
    if (fs::exists(profilePath)) {
    } else
        profilePath = TILE_UI "text4.png";
    LOG_INFO("Load Profile Image: " + profilePath);
    profile->SetDrawable(std::make_shared<Util::Image>(profilePath));
    profile->m_Transform.scale = {3.f, 3.f};
}

void CharacterInfoUIFull::setVisible(bool visible) {
    m_Visible = visible;
    UserInterface::setVisible(visible);
    // if (profile) profile->setVisible(visible);
    if (profile)
        profile->SetVisible(visible);
}

std::vector<std::shared_ptr<Util::GameObject>> CharacterInfoUIFull::getChildren() {
    auto children = UserInterface::getChildren();
    if (profile)
        children.push_back(profile);
    return children;
}
