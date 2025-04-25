#ifndef INTEGRATION_HPP
#define INTEGRATION_HPP

#define TILE_SCALE 4
#define TILE_PIXEL 16
#define TILE_SIZE (TILE_SCALE*TILE_PIXEL)
#define TILE_INTERVAL 250
#define FONT_SIZE 32

#define ASSETS 			"assets/"

#define TILE_ENEMY 		ASSETS "tiles/enemy/"
#define TILE_MAP	 	ASSETS "tiles/map/"
#define TILE_PLAYER 	ASSETS "tiles/player/"
#define TILE_SELECTION  ASSETS "tiles/selection/"
#define TILE_UI 		ASSETS "tiles/user_interface/"

#define DATA_CHARACTER 	ASSETS "data/characters/"
#define DATA_ITEM 	    ASSETS "data/items/"
#define DATA_MAP 		ASSETS "data/level/"

#define FONTPATH  		ASSETS "ref/font/Cubic_11_1.010_R.ttf"

#include <unordered_set>
#include "pch.hpp"
#include "Util/Logger.hpp"
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"

namespace std {
    template <>
    struct hash<glm::ivec2> {
        std::size_t operator()(const glm::ivec2& v) const noexcept {
            std::size_t h1 = std::hash<int>{}(v.x);
            std::size_t h2 = std::hash<int>{}(v.y);
            return h1 ^ (h2 << 1);
        }
    };
}

inline bool operator==(const glm::ivec2& a, const glm::ivec2& b) {
    return a.x == b.x && a.y == b.y;
}

enum class SelectionStatus{
	Normal,
	Moving,
	Locked, //for choose UI
	Targeting,
	Waiting
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

enum class HandHeldItemType {
	Sword,
	Lance,
	Bow,
	Staff,
	Axe
};

class CameraGameObject : public Util::GameObject {
public:
	glm::ivec2 getAbsolutePos() const { return absolutePos; }
	virtual void setAbsolutePos(glm::ivec2 a_pos) { absolutePos = a_pos; }
protected:
	glm::ivec2 absolutePos = { 0, 0 };
};

#endif