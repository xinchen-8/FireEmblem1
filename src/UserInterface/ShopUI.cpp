#include "UserInterface/ShopUI.hpp"

ShopUI::ShopUI(std::vector<std::shared_ptr<Tile>> &tiles) : SelectedUI(tiles) {
    setUISize({7, 7});
    setRelativePos({+floor(PTSD_Config::WINDOW_WIDTH / 2) - 14 * TILE_SIZE,
                    +floor(PTSD_Config::WINDOW_HEIGHT / 2) - 7 * TILE_SIZE});
}

void ShopUI::load(int goldCoin, int level) {
    std::shared_ptr<std::vector<std::vector<std::string>>> weaponData = Tool::inputFile(DATA_ITEM "weapon_data.csv");
    std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile(DATA_ITEM "shop_items.csv");
    weapons.clear();
    options.clear();
    option_flags.clear();
    selectPoint = 0;

    // weaponData->erase(weaponData->begin());
    for (int i = 0; i < 6; i++) {
        std::string name = (*data)[level][i + 1];
        auto &weapon = weaponData->at(HANDHELD_ROWINDEX.at(name));
        weapons.push_back(std::make_shared<Weapon>(weapon));
        options.push_back(name + " /" + std::to_string(weapons.back()->getUses()) + "   ($" +
                          std::to_string(weapons.back()->getWorth()) + ")");
        option_flags.push_back((goldCoin >= weapons.back()->getWorth()));
    }

    std::string str = "";
    for (int i = 0; i < option_flags.size(); i++)
        str += options[i] + "\n";
    setString("Gold Coin: " + std::to_string(goldCoin) + "\n" + str + "\nEnter(buy)\nBackspace(go back)");
    point->setRelativePos(m_Transform.translation + glm::vec2(-TILE_SIZE / 2, 2.7 * TILE_SIZE + 150));
}

void ShopUI::update(int listMov) {
    if (!getVisible())
        return;
    for (int i = selectPoint + listMov; i < option_flags.size() && i >= 0; i += listMov) {
        if (static_cast<bool>(option_flags[i])) {
            selectPoint = i;
            glm::vec2 regPos = point->getRelativePos();
            point->setRelativePos(regPos - glm::vec2(0, 38 * listMov));
            return;
        }
    }
}