#ifndef SELECTION_HPP
#define SELECTION_HPP

#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include "Integration.hpp"
#include "Tile.hpp"
#include "Character/CharacterManager.hpp"

class Character;

class Selection : public CameraGameObject {

public:
	Selection();
	bool moveJudge(Forword forword, glm::ivec2 map_size);
	void moveDirectly(glm::ivec2 move);
	bool isMoving() { return status == SelectionStatus::Moving; }

	void setStatus(SelectionStatus status);
	void setMoveLimit(std::unordered_map<glm::ivec2, int> limit) { limitRange = limit; } //limit with status
	void setSelectCharacter(std::shared_ptr<Character> character);
	void setAnimation() { m_Drawable = (status==SelectionStatus::Normal) ? chooseAnimation : moveAnimation; }
	void clearLimitRange() { limitRange.clear(); }

	std::shared_ptr<Character> getSelectCharacter() { return selectCharacter; }
	glm::ivec2 getOriginalSelectionPos() { return originalSelectionPos; }
	SelectionStatus getStatus() { return status; }
	std::unordered_map<glm::ivec2, int> getLimitRange() { return limitRange; }

private:
	SelectionStatus status = SelectionStatus::Normal;
	std::shared_ptr<Character> selectCharacter = nullptr;
	glm::ivec2 originalSelectionPos = glm::ivec2( 0, 0 );
	
	std::unordered_map<glm::ivec2, int> limitRange = {};

	std::shared_ptr<Util::Animation> chooseAnimation;
	std::shared_ptr<Util::Animation> moveAnimation;
};

#endif