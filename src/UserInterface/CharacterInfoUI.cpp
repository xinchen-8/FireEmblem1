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

ItemInfoUI::ItemInfoUI(std::vector<std::shared_ptr<Tile>> &tiles) : UserInterface(tiles) {
    setUISize({7, 5});

    setRelativePos({+floor(PTSD_Config::WINDOW_WIDTH / 2) - 7 * TILE_SIZE,
                    -floor(PTSD_Config::WINDOW_HEIGHT / 2) + 1 * TILE_SIZE});
}

void ItemInfoUI::load(std::shared_ptr<Item> item) {
    if (item) {
        this->item = item;
        this->character = nullptr;
    }
}

void ItemInfoUI::load(std::shared_ptr<Character> character) {
    if (character) {
        LOG_INFO("Loading character: " + character->getName());
        this->character = character;
        this->currentItemIndex = character->getHandHeldIndex();
        this->item = character->getItems()[currentItemIndex];
        LOG_INFO("Loaded item: " + (item ? item->getName() : "null"));
    } else {
        LOG_INFO("Clearing character and item info");
        this->character = nullptr;
        this->item = nullptr;
        this->currentItemIndex = 0;
    }
}

void ItemInfoUI::nextItem() {
    if (!character) {
        LOG_INFO("No character loaded in ItemInfoUI");
        return;
    }
    
    auto items = character->getItems();
    LOG_INFO("Current item index: " + std::to_string(currentItemIndex));
    for (int i = 1; i <= items.size(); i++) {
        int nextIndex = (currentItemIndex + i) % items.size();
        LOG_INFO("Checking next index: " + std::to_string(nextIndex));
        if (items[nextIndex] != nullptr) {
            currentItemIndex = nextIndex;
            item = items[nextIndex];
            LOG_INFO("Switched to item: " + item->getName());
            update();
            return;
        }
    }
    LOG_INFO("No next item found");
}

// void ItemInfoUI::prevItem() {
//     if (!character) {
//         LOG_INFO("No character loaded in ItemInfoUI");
//         return;
//     }
    
//     auto items = character->getItems();
//     LOG_INFO("Current item index: " + std::to_string(currentItemIndex));
//     for (int i = 1; i <= items.size(); i++) {
//         int prevIndex = (currentItemIndex - i + items.size()) % items.size();
//         LOG_INFO("Checking prev index: " + std::to_string(prevIndex));
//         if (items[prevIndex] != nullptr) {
//             currentItemIndex = prevIndex;
//             item = items[prevIndex];
//             LOG_INFO("Switched to item: " + item->getName());
//             update();
//             return;
//         }
//     }
//     LOG_INFO("No previous item found");
// }

void ItemInfoUI::setItemIndex(int index) {
    if (!character) return;
    
    auto items = character->getItems();
    if (index >= 0 && index < items.size() && items[index] != nullptr) {
        currentItemIndex = index;
        item = items[index];
    }
}

void ItemInfoUI::update() {
    if (!item) {
        setString("No Item");
        return;
    }

    std::string content = item->getName() + "\n";
    auto handHeldItem = std::dynamic_pointer_cast<HandHeldItem>(item);
    if (handHeldItem) {
        content += "Mt " + std::to_string(handHeldItem->getMt()) + "  Hit " + std::to_string(handHeldItem->getHit()) + "\n" +
                  "Crt " + std::to_string(handHeldItem->getCrt()) + "  Rng " + std::to_string(handHeldItem->getRng()[0]) + "\n" +
                  "Wlv " + std::to_string(handHeldItem->getWlv()) + "  Wt " + std::to_string(handHeldItem->getWt()) + "\n";
    }

    auto vulnerary = std::dynamic_pointer_cast<Vulnerary>(item);
    if (vulnerary) {
        content += "Heal 10\n\n\n";  // Vulnerary固定治療10點
    }

    content += "uses " + std::to_string(item->getUses()) + "\n" +
               "Note: \n" + item->getNote();
    
    LOG_INFO("Updating item info for: " + item->getName());
    setString(content);
}

void ItemInfoUI::setVisible(bool visible) {
    m_Visible = visible;
    UserInterface::setVisible(visible);
}

std::vector<std::shared_ptr<Util::GameObject>> ItemInfoUI::getChildren() {
    auto children = UserInterface::getChildren();
    return children;
}