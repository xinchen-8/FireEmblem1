#ifndef TILEINFOUI_HPP
#define TILEINFOUI_HPP
#include "UserInterface.hpp"

class TileInfoUI : public UserInterface {
  public:
    TileInfoUI(std::vector<std::shared_ptr<Tile>> &tiles);
    void load(std::shared_ptr<Tile> tile);
    void update() override;

  private:
    std::shared_ptr<Tile> tile = nullptr;
};

#endif