#include "UserInterface/LoadUI.hpp"

LoadUI::LoadUI(std::vector<std::shared_ptr<Tile>> &tiles) : UserInterface(tiles) {
    setUISize({floor(PTSD_Config::WINDOW_WIDTH / TILE_SIZE) + 2, floor(PTSD_Config::WINDOW_HEIGHT / TILE_SIZE) + 2});
    setRelativePos({-floor(PTSD_Config::WINDOW_WIDTH / 2), -floor(PTSD_Config::WINDOW_HEIGHT / 2)});

    for (auto &r : form) {
        for (auto &t : r) {
            t->SetZIndex(20);
        }
    }

    SetZIndex(22);
    m_Transform.translation = glm::vec2({0, -180});
    m_Transform.scale = {3, 3};

    titlePic->SetDrawable(std::make_shared<Util::Image>(COVERPATH "cover0.png"));
    titlePic->m_Transform.translation = glm::vec2({50, 150});
    titlePic->m_Transform.scale = {3, 3};
    titlePic->SetZIndex(22);

    std::vector<std::string> sp;
    for (int i = 1; i < 9; i++)
        sp.push_back(COVERPATH "cover" + std::to_string(i) + ".png");
    sword->SetDrawable(std::make_shared<Util::Animation>(sp, true, TILE_INTERVAL, true, 0, false));
    sword->m_Transform.translation = glm::vec2(-250, -50);
    sword->m_Transform.scale = {2, 2};
    sword->SetZIndex(21);

    tip->SetDrawable(std::make_shared<Util::Text>(FONTPATH, FONT_SIZE, " ", Util::Color(255, 255, 255), false));
    tip->m_Transform.translation =
        glm::vec2({floor(PTSD_Config::WINDOW_WIDTH / 2) - 200, 50 - floor(PTSD_Config::WINDOW_HEIGHT / 2)});
    tip->SetZIndex(22);
}

bool LoadUI::load(int level) {
    if (level == -1) { // Marth is dead
        setString("Game Over");
        tip->SetDrawable(std::make_shared<Util::Text>(FONTPATH, FONT_SIZE, "Press ESC to leave.",
                                                      Util::Color(255, 255, 255), false));
        titlePic->SetDrawable(std::make_shared<Util::Image>(COVERPATH "gameover0.png"));
        sword->SetDrawable(std::make_shared<Util::Image>(COVERPATH "gameover.png"));

        gameOver = true;
        setVisible(true);
        return false;
    } else if (level > 2) {
        setString("You Finish!");
        tip->SetDrawable(std::make_shared<Util::Text>(FONTPATH, FONT_SIZE, "Press ESC to leave.",
                                                      Util::Color(255, 255, 255), false));
        gameOver = true;
        setVisible(true);
        return false;
    }
    setString("Level " + std::to_string(level));
    tip->SetDrawable(std::make_shared<Util::Text>(FONTPATH, FONT_SIZE, "Press Enter to continue =>",
                                                  Util::Color(255, 255, 255), false));
    setVisible(true);
    return true;
}

void LoadUI::setVisible(bool visible) {
    m_Visible = visible;
    for (auto &row : form) {
        for (auto &e : row) {
            e->SetVisible(visible);
        }
    }
    titlePic->SetVisible(visible);
    sword->SetVisible(visible);
    tip->SetVisible(visible);
}

std::vector<std::shared_ptr<Util::GameObject>> LoadUI::getChildren() {
    std::vector<std::shared_ptr<Util::GameObject>> children;
    for (auto &row : form) {
        for (auto &tile : row) {
            children.push_back(tile);
        }
    }
    children.push_back(titlePic);
    children.push_back(sword);
    children.push_back(tip);
    return children;
}