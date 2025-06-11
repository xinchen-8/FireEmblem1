#include "UserInterface/ShopUI.hpp"

ShopUI::ShopUI(std::vector<std::shared_ptr<Tile>> &tiles) : SelectedUI(tiles) {
    setUISize({7, 7});
    setRelativePos({+floor(PTSD_Config::WINDOW_WIDTH / 2) - 14 * TILE_SIZE,
                    +floor(PTSD_Config::WINDOW_HEIGHT / 2) - 7 * TILE_SIZE});
}

void ShopUI::load(int goldCoin, int level, std::vector<std::string> usableWeapon) {
    std::shared_ptr<std::vector<std::vector<std::string>>> weaponData = Tool::inputFile(DATA_ITEM "weapon_data.csv");
    std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile(DATA_ITEM "shop_items.csv");
    weapons.clear();
    options.clear();
    option_flags.clear();

    for (int i = 0; i < 6; i++) {
        std::string name = (*data)[level][i + 1];
        auto &weapon = weaponData->at(HANDHELD_ROWINDEX.at(name));
        weapons.push_back(std::make_shared<Weapon>(weapon));
        options.push_back(name + " /" + std::to_string(weapons.back()->getUses()) + "   ($" +
                          std::to_string(weapons.back()->getWorth()) + ")");
        option_flags.push_back((goldCoin >= weapons.back()->getWorth()) &&
                               (std::find(usableWeapon.begin(), usableWeapon.end(), weapons.back()->getClassName()) !=
                                usableWeapon.end()));
    }

    std::string str = "";
    for (int i = 0; i < option_flags.size(); i++)
        str += options[i] + "\n";
    setString("Gold Coin: " + std::to_string(goldCoin) + "\n" + str + "\nEnter(buy)\nBackspace(leave)");

    selectPoint = -1;
    for (int i = 0; i < weapons.size(); i++) {
        if (option_flags[i]) {
            selectPoint = i;
            break;
        }
    }

    point->setRelativePos(m_Transform.translation + glm::vec2(-TILE_SIZE / 2, 2.7 * TILE_SIZE + 150) -
                          glm::vec2(0, 38 * selectPoint));
    if (selectPoint == -1)
        point->setVisible(false);
}

void ShopUI::update(int listMov) {
    if (!getVisible())
        return;
    for (int i = selectPoint + listMov; i < option_flags.size() && i >= 0; i += listMov) {
        if (static_cast<bool>(option_flags[i])) {
            selectPoint = i;
            point->setRelativePos(m_Transform.translation + glm::vec2(-TILE_SIZE / 2, 2.7 * TILE_SIZE + 150) -
                                  glm::vec2(0, 38 * selectPoint));
            return;
        }
    }
}

void ShopUI::setVisible(bool visible) {
    m_Visible = visible;
    point->setVisible((selectPoint != -1) ? visible : false);
    for (auto &row : form) {
        for (auto &e : row) {
            e->SetVisible(visible);
        }
    }
}