#ifndef ACTUI_HPP
#define ACTUI_HPP
#include "UserInterface/SelectedUI.hpp"

class ActUI : public SelectedUI {
  public:
    ActUI(std::vector<std::shared_ptr<Tile>> &tiles);
    void load(std::vector<bool> flags, bool targetIsPlayer) override;
    std::string getActive() { return options[selectPoint]; }
};
#endif