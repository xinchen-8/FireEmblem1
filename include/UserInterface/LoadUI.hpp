#ifndef LEVELUI_HPP
#define LEVELUI_HPP
#include "UserInterface/UserInterface.hpp"

class LoadUI : public UserInterface {
  public:
    LoadUI(std::vector<std::shared_ptr<Tile>> &tiles);
    bool load(int level);
    void setVisible(bool visible) override;
    std::vector<std::shared_ptr<Util::GameObject>> getChildren() override;
    bool getGameOver() { return gameOver; }

  private:
    bool gameOver = false;
    std::shared_ptr<Util::GameObject> titlePic = std::make_shared<Util::GameObject>();
    std::shared_ptr<Util::GameObject> sword = std::make_shared<Util::GameObject>();
    std::shared_ptr<Util::GameObject> tip = std::make_shared<Util::GameObject>();
};

#endif