#ifndef SELECTION_HPP
#define SELECTION_HPP

#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include "Integration.hpp"
#include "Tile.hpp"
#include "Character.hpp"

class Selection : public CameraGameObject {

public:
	Selection();
	void changeStatus();
	void setMoveLimit(std::unordered_map<glm::vec2, int> limit) { limitRange = limit; } //limit with status
	

	void setAnimation() { m_Drawable = (status==SelectionStatus::Moving) ? moveAnimation : chooseAnimation; }
	void hideAnimation() { m_Visible = false; }
	void displayAnimation() { m_Visible = true; }
	
	void moveDirectly(glm::vec2 move);
	bool isMoving() { return status == SelectionStatus::Moving; }
private:
	SelectionStatus status = SelectionStatus::Normal;
	std::unordered_map<glm::vec2, int> limitRange = {};
	std::shared_ptr<Util::Animation> chooseAnimation;
	std::shared_ptr<Util::Animation> moveAnimation;
};

#endif