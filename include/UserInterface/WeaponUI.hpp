#ifndef WEAPONUI_HPP
#define WEAPONUI_HPP
#include "UserInterface/SelectedUI.hpp"

class WeaponUI : public SelectedUI {
  public:
    WeaponUI(std::vector<std::shared_ptr<Tile>> &tiles);
    void loadWeapon(std::vector<std::shared_ptr<HandHeldItem>> HHitems);
};
#endif