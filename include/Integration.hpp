#ifndef INTEGRATION_HPP
#define INTEGRATION_HPP

#define TILE_SCALE 4
#define TILE_PIXEL 16
#define TILE_SIZE TILE_SCALE*TILE_PIXEL
#define TILE_INTERVAL 250
#define FONT_SIZE 32

#define ASSETS "assets/"
#define TILES_FOLDER "tiles/tiles_map/"
#define UI_FOLDER "tiles/tiles_UI/"
#define MAP_FOLDER "data/level/"
#define SELECTION_FOLDER "tiles/tiles_selection/"
#define FONTPATH "ref/font/Cubic_11_1.010_R.ttf"

#include "pch.hpp"
#include "Util/Logger.hpp"
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"

enum class Accessable {
	Walkable,
	Swimmable,
	Flyable,
	Inaccessible
};

enum class E_AtkWeapon {
	Sword,
	Lance,
	Bow,
	Staff,
	Axe
};

class CameraGameObject : public Util::GameObject {
public:
	glm::vec2 getAbsolutePos() const { return absolutePos; }
	virtual void setAbsolutePos(glm::vec2 a_pos) { absolutePos = a_pos; }
protected:
	glm::vec2 absolutePos = { 0, 0 };
};

#endif