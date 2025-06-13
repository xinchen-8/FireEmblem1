#ifndef ITEMINFOUI_HPP
#define ITEMINFOUI_HPP
#include "UserInterface.hpp"

class ItemInfoUI : public UserInterface {
  public:
    ItemInfoUI(std::vector<std::shared_ptr<Tile>> &tiles);
    void load(std::shared_ptr<Item> item);
    void load(std::shared_ptr<Character> character);
    void update() override;
    void setVisible(bool visible);
    std::vector<std::shared_ptr<GameObject>> getChildren() override;
    void nextItem();  // 切換到下一個物品
    void setItemIndex(int index);  // 設定當前物品索引

  private:
    std::shared_ptr<Item> item = nullptr;
    int currentItemIndex = 0;  // 當前顯示的物品索引
    std::shared_ptr<Character> character = nullptr;  // 持有物品的角色
};

#endif
