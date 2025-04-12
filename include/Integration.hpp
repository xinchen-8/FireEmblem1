#ifndef INTEGRATION_HPP
#define INTEGRATION_HPP

#define TILE_SCALE 4
#define TILE_PIXEL 16
#define TILE_SIZE (TILE_SCALE*TILE_PIXEL)
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

namespace std {
    template <>
    struct hash<glm::ivec2> {
        std::size_t operator()(const glm::ivec2& v) const noexcept {
            std::size_t h1 = std::hash<float>{}(v.x);
            std::size_t h2 = std::hash<float>{}(v.y);
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

enum class WeaponType {
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