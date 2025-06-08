#ifndef LEVELUPUI_HPP
#define LEVELUPUI_HPP

#include "UserInterface.hpp"
#include "Character/Character.hpp"

class LevelUpUI : public UserInterface {
public:
    LevelUpUI(std::vector<std::shared_ptr<Tile>>& tiles);
    void load(std::shared_ptr<Character> character);
    void update() override;
    void setVisible(bool visible);
    std::vector<std::shared_ptr<GameObject>> getChildren() override;

private:
    std::shared_ptr<Character> character = nullptr;
    bool m_Visible = false;
    float m_DisplayTime = 0.0f;
    const float DISPLAY_DURATION = 3.0f; // Display for 3 seconds
};

#endif 