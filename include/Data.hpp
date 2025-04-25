#include "Integration.hpp"

struct CHARACTER_INDEX {
    static constexpr int NAME  = 0;
    static constexpr int CLASS = 1;
    static constexpr int LV    = 2;
    static constexpr int EX    = 3;
    static constexpr int HP    = 4;
    static constexpr int STR   = 5;
    static constexpr int SKL   = 6;
    static constexpr int WLV   = 7;
    static constexpr int SPD   = 8;
    static constexpr int LCK   = 9;
    static constexpr int DEF   = 10;
    static constexpr int RES   = 11;
    static constexpr int MOV   = 12;
};

struct CHARACTER_GROWTH_INDEX {
    static constexpr int NAME  = 0;
    static constexpr int HP    = 1;
    static constexpr int STR   = 2;
    static constexpr int SKL   = 3;
    static constexpr int WLV   = 4;
    static constexpr int SPD   = 5;
    static constexpr int LCK   = 6;
    static constexpr int DEF   = 7;
    static constexpr int RES   = 8;
};

struct COST_INDEX {
    static constexpr int CLASS    = 0;
    static constexpr int DEFAULT  = 1;
    static constexpr int PLAIN    = 2;
    static constexpr int FOREST   = 3;
    static constexpr int DESERT   = 4;
    static constexpr int PILLAR   = 5;
    static constexpr int FORT     = 6;
    static constexpr int GATE     = 7;
    static constexpr int MOUNTAIN = 8;
    static constexpr int ST_WALL  = 9;
    static constexpr int RIVER    = 10;
    static constexpr int SEA      = 11;
    static constexpr int PEAK     = 12;
    static constexpr int WALL     = 13;
};

struct HANDHELD_INDEX {
    static constexpr int NAME   = 0;
    static constexpr int CLASS  = 1;
    static constexpr int MT     = 2;
    static constexpr int WT     = 3;
    static constexpr int HIT    = 4;
    static constexpr int USES   = 5;
    static constexpr int RNG    = 6;
    static constexpr int WLV    = 7;
    static constexpr int WORTH  = 8;
    static constexpr int CRT    = 9;
    static constexpr int NOTE   = 10;
};

struct ITEM_INDEX {
    static constexpr int NAME  = 0;
    static constexpr int USES  = 1;
    static constexpr int WORTH = 2;
    static constexpr int NOTE  = 3;
};

struct ITEM_ROWINDEX{
    static constexpr int VULNERARY = 1;
};

struct HANDHELD_ROWINDEX{
    static constexpr int RAPIER       = 1;
    static constexpr int IRON_SWORD   = 2;
    static constexpr int STEEL_SWORD  = 3;
    static constexpr int IRON_LANCE   = 4;
    static constexpr int SILVER_LANCE = 5;
    static constexpr int JAVELIN      = 6;
    static constexpr int IRON_BOW     = 7;
    static constexpr int STEEL_BOW    = 8;
    static constexpr int BOWGUN       = 9;
    static constexpr int HEAL         = 10;
    static constexpr int IRON_AXE     = 11;
    static constexpr int STEEL_AXE    = 12;
    static constexpr int HAND_AXE     = 13;
    static constexpr int HAMMER       = 14;
};

// For tiles of map that do not display animation
const std::vector<std::unordered_set<glm::ivec2>> TILE_MASK = {
    {{4,8},{27,4},{22,9}} //the original (0, 0) is left top on map.csv
};
// For tiles of map that characters can not move to it
const std::vector<std::unordered_set<glm::ivec2>> CHARACTER_UNMOVE ={
    {{4*TILE_SIZE, (14-8)*TILE_SIZE},
    {27*TILE_SIZE, (14-4)*TILE_SIZE}}
     //the original (0, 0) is left top on map.csv
};