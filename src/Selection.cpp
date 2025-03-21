#include "Selection.hpp"

Selection::Selection() {
	std::vector<std::string> paths;
	paths.push_back(ASSETS "user/selection0.png");
	paths.push_back(ASSETS "user/transparent.png");
	chooseAnimation = std::make_shared<Util::Animation>(paths, true, TILE_INTERVAL, true, 0);
	m_Drawable = chooseAnimation;
	m_Transform.scale = { TILE_SCALE, TILE_SCALE };
	m_ZIndex = 4;

	paths[0] = ASSETS "user/selection1.png";
	moveAnimation = std::make_shared<Util::Animation>(paths, true, TILE_INTERVAL, true, 0);
}