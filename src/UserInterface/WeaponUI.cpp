#include "UserInterface/WeaponUI.hpp"

WeaponUI::WeaponUI(std::vector<std::shared_ptr<Tile>> &tiles) : SelectedUI(tiles) {
    setUISize({6, 4});
    setRelativePos({+floor(PTSD_Config::WINDOW_WIDTH / 2) - 6 * TILE_SIZE,
                    +floor(PTSD_Config::WINDOW_HEIGHT / 2) - 4 * TILE_SIZE});
}

void WeaponUI::loadWeapon(std::vector<std::shared_ptr<HandHeldItem>> HHitems) {
    // std::cout<<std::to_string(weapons.size())<<std::endl;
    options.clear();
    option_flags.clear();

    for (auto &i : HHitems) {
        if (i) {
            // std::ostringstream reg;
            // reg << std::left << std::setw(15) << w->getName()
            // << std::right << std::setw(3) << w->getUses();
            options.push_back(i->getName() + " /" + std::to_string(i->getUses()));
            option_flags.push_back(true);
        } else {
            options.push_back("null");
            option_flags.push_back(false);
        }
    }

    // for(auto &t: options) std::cout<<t<<std::endl;
    // for(const auto &t: option_flags) std::cout<<std::to_string(t)<<std::endl;

    for (int i = 0; i < option_flags.size(); i++) {
        if (option_flags[i]) {
            selectPoint = i;
            break;
        }
    }

    std::string str = "";
    int counter = 0;
    for (int i = 0; i < option_flags.size(); i++) {
        if (option_flags[i]) {
            str += options[i] + "\n";
            counter++;
        }
    }

    int reg = 5 - counter;
    while (reg-- > 0)
        str += "\n";

    setString(str);
    point->setRelativePos(m_Transform.translation + glm::vec2(-TILE_SIZE / 2, 2.7 * TILE_SIZE - 40 * selectPoint));
}
