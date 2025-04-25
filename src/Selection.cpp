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
	else if(status == SelectionStatus::Targeting){
		glm::ivec2 deltaPos = {INT_MAX, INT_MAX};
		for(auto &[pos, null]: limitRange){
			if(move.x==TILE_SIZE && (pos.x-absolutePos.x>0 && abs(pos.x-absolutePos.x)<abs(deltaPos.x)))
				deltaPos = pos - absolutePos;
			if(move.x==-TILE_SIZE && (pos.x-absolutePos.x<0 && abs(pos.x-absolutePos.x)<abs(deltaPos.x))) 
				deltaPos = pos - absolutePos;					
			if(move.y==TILE_SIZE &&(pos.y-absolutePos.y>0 && abs(pos.y-absolutePos.y)<abs(deltaPos.y)))
				deltaPos = pos - absolutePos;
			if(move.y==-TILE_SIZE &&(pos.y-absolutePos.y<0 && abs(pos.y-absolutePos.y)<abs(deltaPos.y))) 
				deltaPos = pos - absolutePos;
			}
		if(deltaPos!=glm::ivec2(INT_MAX, INT_MAX)) 
			absolutePos += deltaPos;	
		//else LOG_ERROR("Doesn't have next attacked target ...");
	}

	else if(status != SelectionStatus::Locked) absolutePos += move;
}

void Selection::setStatus(SelectionStatus status){
	this->status = status;
	setAnimation();
}

void Selection::setSelectCharacter(std::shared_ptr<Character> character) {
	selectCharacter = character; 
	originalSelectionPos = absolutePos;
}

