#ifndef ITEMUI_HPP
#define ITEMUI_HPP
#include "UserInterface/SelectedUI.hpp"

class ItemUI : public SelectedUI {
  public:
    ItemUI(std::vector<std::shared_ptr<Tile>> &tiles);
    void loadItem(std::vector<std::shared_ptr<Item>> items, int handheldIndex = -1);
    void update(int listMov) override;
    void VisibleLockItemUI(); // for ShopUI
};
#endif