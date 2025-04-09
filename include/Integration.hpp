#ifndef INTEGRATION_HPP
#define INTEGRATION_HPP

#define TILE_SCALE 4
#define TILE_PIXEL 16
#define TILE_SIZE TILE_SCALE*TILE_PIXEL
#define TILE_INTERVAL 250
#define FONT_SIZE 32

#define ASSETS "assets/"
#define TILES_FOLDER "tiles/tiles_map/"
#define TILES_PLAYER_FOLDER "tiles/tiles_players/"
#define UI_FOLDER "tiles/tiles_UI/"
#define SELECTION_FOLDER "tiles/tiles_selection/"

#define CHARACTER_FOLDER "data/characters/"
#define MAP_FOLDER "data/level/"

#define FONTPATH "ref/font/Cubic_11_1.010_R.ttf"

#include "pch.hpp"
#include "Util/Logger.hpp"
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"


enum class CharacterClass{
	Lord,
	Cavalier,
	Paladin,
	PegasusKnight,
	Mercenary,
	Archer,
	Hunter,
	Knight,
	Fighter,
	Pirate,
	Thief,
	Curate
};

enum class CharacterStatus{
	Normal,
	Moving,
	Waiting
};

enum class Forword{
	Up,
	Down,
	Left,
	Right
};

enum class WeaponType {
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