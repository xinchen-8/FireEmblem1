#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <math.h>
#include "Util/Renderer.hpp"
#include "config.hpp"
#include "Selection.hpp"
#include "Tile.hpp"
#include "UserInterface.hpp"
#include "CharacterManager.hpp"

class Camera {

public:
	Camera(
		std::shared_ptr<PlayerManager> pm,
		std::shared_ptr<EnemyManager> em,
		std::shared_ptr<MapManager> tm,
		std::shared_ptr<UIManager> ui,
		std::shared_ptr<Selection> s
	);
	void resetCameraAbsolutePos();
	void setChildrenRelativePos();
	void update();

private:
	std::vector<std::shared_ptr<CameraGameObject>> children = {};
	std::vector<std::shared_ptr<Util::GameObject>> UIchildren = {};
	std::shared_ptr<PlayerManager> playerManager = nullptr;
	std::shared_ptr<EnemyManager> enemyManager = nullptr;
	std::shared_ptr<MapManager> mapManager = nullptr;
	std::shared_ptr<Selection> selection = nullptr;
	std::shared_ptr<UIManager> uiManager = nullptr;
	Util::Renderer renderer;

	glm::ivec2 absolutePos = { 0, 0 };
};

#endif