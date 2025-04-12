#ifndef SELECTION_HPP
#define SELECTION_HPP

#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include "Integration.hpp"
#include "Tile.hpp"
#include "Character.hpp"

class Character;

class Selection : public CameraGameObject {

public:
	Selection();
	void setStatus(SelectionStatus status);
	void setMoveLimit(std::unordered_map<glm::ivec2, int> limit) { limitRange = limit; } //limit with status
	
	void setSelectCharacter(std::shared_ptr<Character> character) { selectCharacter = character; }
	void setAnimation() { m_Drawable = (status==SelectionStatus::Normal) ? chooseAnimation : moveAnimation; }
	
	void moveDirectly(glm::ivec2 move);
	bool isMoving() { return status == SelectionStatus::Moving; }

	std::shared_ptr<Character> getSelectCharacter() { return selectCharacter; }
	SelectionStatus getStatus() { return status; }

private:
	SelectionStatus status = SelectionStatus::Normal;
	std::shared_ptr<Character> selectCharacter = nullptr;
	std::unordered_map<glm::ivec2, int> limitRange = {};
	
	std::shared_ptr<Util::Animation> chooseAnimation;
	std::shared_ptr<Util::Animation> moveAnimation;
};

#endif