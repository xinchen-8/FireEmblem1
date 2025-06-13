#include "UserInterface/ItemInfoUI.hpp"
namespace fs = std::filesystem;

ItemInfoUI::ItemInfoUI(std::vector<std::shared_ptr<Tile>> &tiles) : UserInterface(tiles) {
    setUISize({8, 6});

    setRelativePos({+floor(PTSD_Config::WINDOW_WIDTH / 2) - 8 * TILE_SIZE,
                    -floor(PTSD_Config::WINDOW_HEIGHT / 2) + 1 * TILE_SIZE});
    m_Transform.translation -= glm::ivec2(0, TILE_SIZE / 5);
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
    if (!character)
        return;

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
        auto reg = std::to_string(handHeldItem->getRng()[0]);
        reg += (handHeldItem->getRng()[0] == handHeldItem->getRng().back())
                   ? " "
                   : ("~" + std::to_string(handHeldItem->getRng().back()));

        content += "Mt " + std::to_string(handHeldItem->getMt()) + "  Hit " + std::to_string(handHeldItem->getHit()) +
                   "\n" + "Crt " + std::to_string(handHeldItem->getCrt()) + "  Rng " + reg + "\n" + "Wlv " +
                   std::to_string(handHeldItem->getWlv()) + "  Wt " + std::to_string(handHeldItem->getWt()) + "\n";
    }

    auto vulnerary = std::dynamic_pointer_cast<Vulnerary>(item);
    if (vulnerary) {
        content += "Heal 10\n\n\n\n"; // Vulnerary固定治療10點
    }
    
    // 處理 Note 文字換行
    std::string note = item->getNote();
    const int maxWidth = 22; // 每行最大字數
    std::string wrappedNote;
    int currentWidth = 0;
    int linecount = 0;
    
    for (size_t i = 0; i < note.length(); i++) {
        if (currentWidth >= maxWidth && note[i] == ' ') {
            wrappedNote += "\n";
            currentWidth = 0;
            linecount++;
        } else {
            wrappedNote += note[i];
            currentWidth++;
        }
    }
    LOG_INFO("Linecount: " + std::to_string(linecount));

    if (linecount == 2) {
        content += "uses " + std::to_string(item->getUses()) + "\n" + "Note: \n" + wrappedNote;
    }
    else {
        for (int i = 0; i < 2 - linecount; i++) {
            content += "\n";
        }
        content += "uses " + std::to_string(item->getUses()) + "\n" + "Note: \n" + wrappedNote;
    }

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
