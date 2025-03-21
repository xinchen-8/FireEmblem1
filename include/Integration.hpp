#ifndef INTEGRATION_HPP
#define INTEGRATION_HPP

#define TILE_SCALE 4
#define TILE_SIZE 16
#define TILE_INTERVAL 150
#define ASSETS "../../../assets/"


#include "pch.hpp"
#include "Util/Logger.hpp"
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"

enum class Accessable {
	Walkable,
	Swimmable,
	Flyable
};

class CameraGameObject : public Util::GameObject {
public:
	glm::vec2 getAbsolutePos() const { return absolutePos; }
	void setAbsolutePos(glm::vec2 a_pos) { absolutePos = a_pos; }
protected:
	glm::vec2 absolutePos = { 0, 0 };
};

#endif