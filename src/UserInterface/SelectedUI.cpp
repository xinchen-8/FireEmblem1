#include "UserInterface/SelectedUI.hpp"

SelectedUI::SelectedUI(std::vector<std::shared_ptr<Tile>> &tiles) : UserInterface(tiles) {
    point->SetZIndex(8);
    setVisible(false);
}

void SelectedUI::setVisible(bool visible) {
    m_Visible = visible;
    point->setVisible(visible);
    for (auto &row : form) {
        for (auto &e : row) {
            e->SetVisible(visible);
        }
    }
}

void SelectedUI::update(int listMov) {
    if (!getVisible())
        return;
    for (int i = selectPoint + listMov; i < option_flags.size() && i >= 0; i += listMov) {
        if (static_cast<bool>(option_flags[i])) {
            selectPoint = i;
            glm::vec2 regPos = point->getRelativePos();
            point->setRelativePos(regPos - glm::vec2(0, 40 * listMov));
            return;
        }
    }
}

std::vector<std::shared_ptr<Util::GameObject>> SelectedUI::getChildren() {
    std::vector<std::shared_ptr<Util::GameObject>> children;
    children.push_back(point);
    for (auto &row : form) {
        for (auto &tile : row) {
            children.push_back(tile);
        }
    }
    return children;
}
