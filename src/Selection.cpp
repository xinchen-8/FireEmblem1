#include "Selection.hpp"

Selection::Selection(){
	std::vector<std::string> paths;
	paths.push_back(ASSETS SELECTION_FOLDER "selection0.png");
	paths.push_back(ASSETS SELECTION_FOLDER "transparent.png");
	chooseAnimation = std::make_shared<Util::Animation>(paths, true, TILE_INTERVAL, true, 0);
	m_Drawable = chooseAnimation;
	m_Transform.scale = { TILE_SCALE, TILE_SCALE };
	m_ZIndex = 5;

	paths[0] = ASSETS SELECTION_FOLDER "selection1.png";
	moveAnimation = std::make_shared<Util::Animation>(paths, true, TILE_INTERVAL, true, 0);
}

void Selection::changeStatus(){
	status = (status == SelectionStatus::Normal) ? 
		SelectionStatus::Moving : SelectionStatus::Normal;
	setAnimation();
}

void Selection::moveDirectly(glm::vec2 move){
	if(status == SelectionStatus::Moving){
		glm::vec2 newPos = absolutePos + move;
		if(limitRange.find(newPos) != limitRange.end()) absolutePos = newPos;
	}
	else absolutePos += move;
}