#include "Camera.hpp"

Camera::Camera(
	std::shared_ptr<TileManager> tm,
	std::shared_ptr<UIManager> ui,
	std::shared_ptr<Selection> s) : 
	
	tileManager(tm), uiManager(ui), selection(s) {
	std::vector<std::shared_ptr<CameraGameObject>> tiles =  tileManager->getChildren();
	children.insert(children.end(), tiles.begin(), tiles.end());
	children.push_back(selection);

	std::vector<std::shared_ptr<Util::GameObject>> ui_tiles = uiManager->getChildren();
	UIchildren.insert(UIchildren.end(), ui_tiles.begin(), ui_tiles.end());

	for (auto& element : children) {
		renderer.AddChild(element);
	}
	for (auto& element : UIchildren) {
		renderer.AddChild(element);
	}
	
	glm::vec2 BorderDistance = {
		round(PTSD_Config::WINDOW_WIDTH / (TILE_SIZE * 2)) * TILE_SIZE,
		round(PTSD_Config::WINDOW_HEIGHT / (TILE_SIZE * 2)) * TILE_SIZE
	};
	glm::vec2 mapSize = tileManager->getMapSize();

	absolutePos = selection->getAbsolutePos();
	if (absolutePos.x < BorderDistance.x)
		absolutePos.x = BorderDistance.x;
	else if(absolutePos.x >= mapSize.x - BorderDistance.x - TILE_SIZE)
		absolutePos.x = mapSize.x - BorderDistance.x - TILE_SIZE;
	if (absolutePos.y < BorderDistance.y)
		absolutePos.y = BorderDistance.y;
	else if(absolutePos.y >= mapSize.y - BorderDistance.y - TILE_SIZE)
		absolutePos.y = mapSize.y - BorderDistance.y - TILE_SIZE;
}


void Camera::setChildrenRelativePos() {
	for (auto& element : children) {
		element->m_Transform.translation = element->getAbsolutePos() - absolutePos;
	}
}


void Camera::update() {
	glm::vec2 BorderDistance = {
		round(PTSD_Config::WINDOW_WIDTH / (TILE_SIZE * 2)) * TILE_SIZE,
		round(PTSD_Config::WINDOW_HEIGHT / (TILE_SIZE * 2)) * TILE_SIZE
	};
	glm::vec2 mapSize = tileManager->getMapSize();
	glm::vec2 selectionPos = selection->getAbsolutePos();

	if (selectionPos.x >= BorderDistance.x && selectionPos.x <= mapSize.x - BorderDistance.x - TILE_SIZE)
		absolutePos.x = selectionPos.x;
	if (selectionPos.y >= BorderDistance.y && selectionPos.y <= mapSize.y - BorderDistance.y - TILE_SIZE)
		absolutePos.y = selectionPos.y;

	setChildrenRelativePos();
	renderer.Update();
}
