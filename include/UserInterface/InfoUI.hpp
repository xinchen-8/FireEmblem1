#ifndef INFOUI_HPP
#define INFOUI_HPP
#include "UserInterface.hpp"

class TileInfoUI : public UserInterface {
  public:
    TileInfoUI(std::vector<std::shared_ptr<Tile>> &tiles);
    void load(std::shared_ptr<Tile> tile);
    void update() override;

  private:
    std::shared_ptr<Tile> tile = nullptr;
};

// class CharacterInfoUI : public UserInterface {
// public:
// 	CharacterInfoUI(std::vector<std::shared_ptr<Tile>>& tiles);
// 	void load(std::shared_ptr<Character> character);
// 	void update() override;
// private:
// 	std::shared_ptr<Character> character = nullptr;
// };

class CharacterInfoUIFull : public UserInterface {
  public:
    CharacterInfoUIFull(std::vector<std::shared_ptr<Tile>> &tiles);
    void load(std::shared_ptr<Character> character);
    void update() override;
    void setVisible(bool visible);
    std::vector<std::shared_ptr<GameObject>> getChildren() override;

  private:
    // std::shared_ptr<ImageUI> profile;
    std::shared_ptr<Util::GameObject> profile = nullptr;
    std::shared_ptr<Character> character = nullptr;
};

class ItemInfoUI : public UserInterface {
  public:
    ItemInfoUI(std::vector<std::shared_ptr<Tile>> &tiles);
    void load(std::shared_ptr<Item> item);
    void update() override;
    void setVisible(bool visible);
    std::vector<std::shared_ptr<GameObject>> getChildren() override;

  private:
    std::shared_ptr<Item> item = nullptr;
};

#endif