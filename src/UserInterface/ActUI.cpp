#include "UserInterface/ActUI.hpp"

ActUI::ActUI(std::vector<std::shared_ptr<Tile>> &tiles) : SelectedUI(tiles) {
    setUISize({4, 4});
    setRelativePos({+floor(PTSD_Config::WINDOW_WIDTH / 2) - 5 * TILE_SIZE,
                    +floor(PTSD_Config::WINDOW_HEIGHT / 2) - 4 * TILE_SIZE});

    options = {"Next", "Visit", "Talk", "Attack", "Item", "Wait"}; //"Trade"
    option_flags = {0, 0, 0, 0, 1, 1};
}

void ActUI::load(std::vector<bool> flags, bool targetIsPlayer) {
    options[3] = (targetIsPlayer) ? "Target" : "Attack";

    if (flags.size() == option_flags.size()) {
        option_flags = flags;
        for (int i = 0; i < option_flags.size(); i++) {
            if (option_flags[i]) {
                selectPoint = i;
                break;
            }
        }

        int counter = 0;
        std::string str = "";
        for (int i = 0; i < option_flags.size(); i++) {
            if (option_flags[i]) {
                str += options[i] + "\n";
                counter++;
            }
        }
        int spaceNum = 4 - counter;

        while (spaceNum-- >= 0)
            str += "\n";
        setString(str);
        point->setRelativePos(m_Transform.translation + glm::vec2(-TILE_SIZE / 2, 2.7 * TILE_SIZE)); // offset
    } else
        LOG_ERROR("Not Correct Input Flags!");
}
