#ifndef SELECTION_HPP
#define SELECTION_HPP

#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include "Integration.hpp"

class Selection : public CameraGameObject {

public:
	Selection();
	void setAnimation(bool moving) { m_Drawable = (moving) ? moveAnimation : chooseAnimation; }
	void hideAnimation() { m_Visible = false; }
	void displayAnimation() { m_Visible = true; }
	void moveDirectly(glm::vec2 move) { absolutePos += move; }
	bool isMoving() { return m_Drawable == moveAnimation; }
private:
	std::shared_ptr<Util::Animation> chooseAnimation;
	std::shared_ptr<Util::Animation> moveAnimation;
};

#endif