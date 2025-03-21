#include "Camera.hpp"

Camera::Camera(std::shared_ptr<TileManager> tm, std::shared_ptr<Selection> s) :
tileManager(tm), selection(s){
	std::vector<std::shared_ptr<CameraGameObject>> tiles =  tileManager->getChildren();
	children.insert(children.end(), tiles.begin(), tiles.end());
	children.push_back(selection);

	for (auto& element : children) {
		renderer.AddChild(element);
	}
}


void Camera::setChildrenRelativePos() {
	for (auto& element : children) {
		element->m_Transform.translation = element->getAbsolutePos() - absolutePos;
	}
}


void Camera::update() {
	absolutePos = selection->getAbsolutePos();
	setChildrenRelativePos();
	renderer.Update();
}