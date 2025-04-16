#include "Selection.hpp"

Selection::Selection(){
	std::vector<std::string> paths;
	paths.push_back(TILE_SELECTION "selection0.png");
	paths.push_back(TILE_SELECTION "transparent.png");
	chooseAnimation = std::make_shared<Util::Animation>(paths, true, TILE_INTERVAL, true, 0);
	m_Drawable = chooseAnimation;
	m_Transform.scale = { TILE_SCALE, TILE_SCALE };
	m_ZIndex = 5;

	paths[0] = TILE_SELECTION "selection1.png";
	moveAnimation = std::make_shared<Util::Animation>(paths, true, TILE_INTERVAL, true, 0);
}

bool Selection::moveJudge(Forword forword, glm::ivec2 map_size){
	if(status == SelectionStatus::Waiting) return false;
	switch(forword){
		case Forword::Up:
			if(absolutePos.y < map_size.y - TILE_SIZE) return true;
			break;
		case Forword::Down:
			if(absolutePos.y > 0) return true;
			break;
		case Forword::Left:
			if(absolutePos.x > 0) return true;
			break;
		case Forword::Right:
			if(absolutePos.x < map_size.x - TILE_SIZE) return true;
			break;
	}
	return false;
}

void Selection::moveDirectly(glm::ivec2 move){
	if(status == SelectionStatus::Moving){
		glm::ivec2 newPos = absolutePos + move;
		if(limitRange.find(newPos) != limitRange.end()) absolutePos = newPos;
	}
	else absolutePos += move;
}

void Selection::setStatus(SelectionStatus status){
	this->status = status;
	setAnimation();
}

void Selection::setSelectCharacter(std::shared_ptr<Character> character) {
	selectCharacter = character; 
	originalSelectionPos = absolutePos;
}

