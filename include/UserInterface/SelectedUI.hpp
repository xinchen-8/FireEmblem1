#ifndef SELECTEDUI_HPP
#define SELECTEDUI_HPP
#include "UserInterface.hpp"

class SelectedUI : public UserInterface { // base on ActUI
  public:
    SelectedUI(std::vector<std::shared_ptr<Tile>> &tiles);
    void setVisible(bool visible) override;
    virtual void load(std::vector<bool> flags, bool targetIsPlayer) {};
    virtual void update(int listMov);

    std::string getActive() { return options[selectPoint]; }
    int getSelectPointIndex() { return selectPoint; }
    std::vector<std::shared_ptr<GameObject>> getChildren() override;

  protected:
    std::vector<std::string> options;
    std::vector<bool> option_flags;
    int selectPoint = 0;

    std::shared_ptr<Tile> point = std::make_shared<Tile>("Point", TILE_SELECTION "point.png");
};

#endif