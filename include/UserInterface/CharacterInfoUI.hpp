#ifndef CHARACTERINFOUI_HPP
#define CHARACTERINFOUI_HPP
#include "UserInterface.hpp"

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
    std::shared_ptr<Util::GameObject> profile = nullptr;
    std::shared_ptr<Character> character = nullptr;
};
#endif