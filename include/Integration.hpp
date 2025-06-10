#ifndef INTEGRATION_HPP
#define INTEGRATION_HPP

#define TILE_SCALE 4
#define TILE_PIXEL 16
#define TILE_SIZE (TILE_SCALE * TILE_PIXEL)
#define TILE_INTERVAL 250
#define FONT_SIZE 32

#define ASSETS "assets/" // ../../../assets for vs

#define COVERPATH ASSETS "ref/cover/"

#define TILE_ENEMY ASSETS "tiles/enemy/"
#define TILE_MAP ASSETS "tiles/map/"
#define TILE_PLAYER ASSETS "tiles/player/"
#define TILE_SELECTION ASSETS "tiles/selection/"
#define TILE_UI ASSETS "tiles/user_interface/"
#define TILE_PROFILE ASSETS "tiles/profile/"

#define BATTLE_PLAYER ASSETS "battle/player/"
#define BATTLE_ENEMY ASSETS "battle/enemy/"

#define DATA_CHARACTER ASSETS "data/characters/"
#define DATA_ITEM ASSETS "data/items/"
#define DATA_MAP ASSETS "data/level/"

#define FONTPATH ASSETS "ref/font/Cubic_11_1.010_R.ttf"

#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"
#include "Util/Logger.hpp"
#include "pch.hpp"
#include <unordered_set>

namespace std {
template <> struct hash<glm::ivec2> {
    std::size_t operator()(const glm::ivec2 &v) const noexcept {
        std::size_t h1 = std::hash<int>{}(v.x);
        std::size_t h2 = std::hash<int>{}(v.y);
        return h1 ^ (h2 << 1);
    }
};
} // namespace std

inline bool operator==(const glm::ivec2 &a, const glm::ivec2 &b) { return a.x == b.x && a.y == b.y; }

enum class SelectionStatus {
    Normal,
    Moving,
    Walking,
    SUI,
    Targeting,
    HUI,
    AttackTargeting,
    AttackWUI,
    BattleUI,
    ITEMIUI,
    ShopUI,
    loadUI,
    EnemyMoving
};

enum class CharacterStatus { Normal, Moving, Waiting };

enum class Forword { Up, Down, Left, Right };

enum class HandHeldItemType { Sword, Lance, Bow, Staff, Axe };

enum class CharacterInfoMode {
    Summary, // basic info
    Full     // all info
};

class CameraGameObject : public Util::GameObject {
  public:
    glm::ivec2 getAbsolutePos() const { return absolutePos; }
    virtual void setAbsolutePos(glm::ivec2 a_pos) { absolutePos = a_pos; }

  protected:
    glm::ivec2 absolutePos = {0, 0};
};

#endif