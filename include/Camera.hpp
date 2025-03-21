#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <math.h>
#include "Util/Renderer.hpp"
#include "config.hpp"
#include "Selection.hpp"
#include "Tile.hpp"

class Camera {

public:
	Camera(std::shared_ptr<TileManager> tm, std::shared_ptr<Selection> s);
	void setChildrenRelativePos();
	void update();
private:
	std::vector<std::shared_ptr<CameraGameObject>> children = {};
	std::shared_ptr<TileManager> tileManager = nullptr;
	std::shared_ptr<Selection> selection = nullptr;
	Util::Renderer renderer;

	glm::vec2 absolutePos = { 0, 0 };
};

#endif