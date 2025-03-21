#ifndef TILE_HPP
#define TILE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Integration.hpp"

namespace Tool {
    std::shared_ptr<std::vector<std::vector<std::string>>> inputFile(std::string file);
}


class Tile : public CameraGameObject{

public:
    Tile(std::vector<std::string> t_list);
    Tile(const Tile& other, glm::vec2 a_pos);
    bool Access(Accessable accessable);

    void setRelativePos(glm::vec2 r_pos) { m_Transform.translation = r_pos; }
    void setStop(){ std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->Pause(); }
    void setStart(){ std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->Play(); }
    
    glm::vec2 getRelativePos() const { return m_Transform.translation; }
    int getAvoid() { return avoid; }
    int getCost() { return cost; }

private:
    std::string name = "";
    int avoid = 0;
    int cost = 0;
    Accessable access = Accessable::Walkable;
};


class TileManager{
public:
    TileManager(int level);
    void buildTileTable();
    void loadMap(int level);
    
    void startAnimations();
    void stopAnimations();
    void setAbsolutePos(glm::vec2 a_pos) { absolutePos = a_pos; }
    
    glm::vec2 getTilesAbsolutePos(glm::vec2 pos);
    glm::vec2 getMapSize() const { return glm::vec2(map[0].size() * TILE_SIZE * TILE_SCALE, map.size() * TILE_SIZE * TILE_SCALE); }
    glm::vec2 getAbsolutePos() const { return absolutePos; }
    int getLevel() const { return level; }
    std::shared_ptr<Tile> getTile(glm::vec2 pos);
    std::vector<std::shared_ptr<CameraGameObject>> getChildren();

private:
    int level = 0;
    glm::vec2 tileNum = {0, 0};
    glm::vec2 absolutePos = {0, 0}; //main point of camera is down left corner of map
    std::unordered_map<std::string, std::shared_ptr<Tile>> tileTable;
    std::vector<std::vector<std::shared_ptr<Tile>>> map = {};
};

#endif