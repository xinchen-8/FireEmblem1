#include "Integration.hpp"

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