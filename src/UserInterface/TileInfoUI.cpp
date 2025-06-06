#include "UserInterface/TileInfoUI.hpp"
#include <filesystem>

TileInfoUI::TileInfoUI(std::vector<std::shared_ptr<Tile>> &tiles) : UserInterface(tiles) {
    setUISize({4, 2});

    setRelativePos({-floor(PTSD_Config::WINDOW_WIDTH / 2) + 1 * TILE_SIZE,
                    -floor(PTSD_Config::WINDOW_HEIGHT / 2) + 1 * TILE_SIZE});
}

void TileInfoUI::load(std::shared_ptr<Tile> tile) {
    if (tile)
        this->tile = tile;
}

void TileInfoUI::update() {
    setString(tile->getName() + "\navoid +" + std::to_string(tile->getAvoid()));
    // setAbsolutePos({ TILE_SIZE, TILE_SIZE });
}
