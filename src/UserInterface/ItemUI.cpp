#include "UserInterface/ItemUI.hpp"

ItemUI::ItemUI(std::vector<std::shared_ptr<Tile>> &tiles) : SelectedUI(tiles) {
    setUISize({6, 3});
    m_Transform.scale = {1, 0.8};
    setRelativePos({+floor(PTSD_Config::WINDOW_WIDTH / 2) - 7 * TILE_SIZE,
                    +floor(PTSD_Config::WINDOW_HEIGHT / 2) - 4 * TILE_SIZE});
}

void ItemUI::loadItem(std::vector<std::shared_ptr<Item>> items, int handheldIndex) {
    options.clear();
    option_flags.clear();

    for (auto &w : items) {
        if (w != nullptr) {
            options.push_back(w->getName() + " " + std::to_string(w->getUses()));
            option_flags.push_back(true);
        } else {
            options.push_back("null");
            option_flags.push_back(false);
        }
    }

    std::string str = "";
    for (int i = 0; i < option_flags.size(); i++)
        str += options[i] + "\n";
    setString(str);
    if (items[handheldIndex])
        selectPoint = handheldIndex;
    point->setRelativePos(m_Transform.translation + glm::vec2(-TILE_SIZE / 2, 1.65 * TILE_SIZE - handheldIndex * 30));
}

void ItemUI::update(int listMov) {
    if (!getVisible())
        return;
    for (int i = selectPoint + listMov; i < option_flags.size() && i >= 0; i += listMov) {
        if (static_cast<bool>(option_flags[i])) {
            selectPoint = i;
            glm::vec2 regPos = point->getRelativePos();
            point->setRelativePos(regPos - glm::vec2(0, 30 * listMov));
            return;
        }
    }
}