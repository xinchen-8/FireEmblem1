#ifndef SHOPUI_HPP
#define SHOPUI_HPP

#include "Item.hpp"
#include "UserInterface/SelectedUI.hpp"

class ShopUI : public SelectedUI {
  public:
    ShopUI(std::vector<std::shared_ptr<Tile>> &tiles);
    void load(int goldCoin, int level, std::vector<std::string> usableWeapon);
    void update(int listMov) override;
    std::shared_ptr<Weapon> getWeapon() { return (selectPoint == -1) ? nullptr : weapons[selectPoint]; }
    void setVisible(bool visible) override;

  private:
    std::vector<std::shared_ptr<Weapon>> weapons = {};
};

#endif