#ifndef UI_HPP
#define UI_HPP

#include "Character/CharacterManager.hpp"
#include "Integration.hpp"
#include "Selection.hpp"
#include "Tile.hpp"
#include "Util/Text.hpp"
#include "config.hpp"


class UserInterface : public Util::GameObject {
  public:
    UserInterface(const std::vector<std::shared_ptr<Tile>> &tiles);

    virtual void update() { return; }
    virtual void setVisible(bool visible);
    void setRelativePos(glm::ivec2 r_pos);
    void setRelativePos();
    void setUISize(glm::ivec2 windowNums);
    void setString(std::string content);

    glm::ivec2 getTileRelativePos(glm::ivec2 pos);
    glm::ivec2 getRelativePos() { return m_Transform.translation; }
    glm::ivec2 getUISize() { return {UItileNum.x * TILE_SIZE, UItileNum.y * TILE_SIZE}; }
    bool getVisible() const { return m_Visible; }
    virtual std::vector<std::shared_ptr<GameObject>> getChildren();

  protected:
    glm::ivec2 UItileNum = {0, 0};
    std::vector<std::vector<std::shared_ptr<Tile>>> form = {};
    std::vector<std::shared_ptr<Tile>> tileTable = {};
};

#endif