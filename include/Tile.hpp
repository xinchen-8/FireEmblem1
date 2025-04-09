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
    //for UI
    Tile(std::string id, std::string img_path);
    //for map
    Tile(const Tile& other, glm::vec2 a_pos);
    bool Access() { return access; }

    void setRelativePos(glm::vec2 r_pos) { m_Transform.translation = r_pos; }
    void setStop(){ std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->Pause(); }
    void setStart(){ std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->Play(); }
    void mask(){ access = false; m_Visible = false; }

    glm::vec2 getRelativePos() const { return m_Transform.translation; }
    std::string getName() { return name; }
    int getAvoid() { return avoid; }

private:
    std::string name = "";
    int avoid = 0;
    int cost = 0;
    bool access = true;
};


class MapManager{
public:
    MapManager(int level);
    void buildTileTable();
    void loadMap(int level);
    
    void startAnimations();
    void stopAnimations();
    void setAbsolutePos(glm::vec2 a_pos) { absolutePos = a_pos; }
    
    glm::vec2 getTileAbsolutePos(glm::vec2 pos);
    glm::vec2 getMapSize() const { return glm::vec2(map[0].size() * TILE_SIZE, map.size() * TILE_SIZE); }
    glm::vec2 getAbsolutePos() const { return absolutePos; }
    int getLevel() const { return level; }
    std::shared_ptr<Tile> getPosTile(glm::vec2 a_pos);
    std::shared_ptr<Tile> getTile(std::string id);
    std::vector<std::shared_ptr<CameraGameObject>> getChildren();

private:
    void maskTile(glm::vec2 pos);

    int level = 0;
    glm::vec2 tileNum = {0, 0};
    glm::vec2 absolutePos = {0, 0}; //main point of camera is down left corner of map
    std::unordered_map<std::string, std::shared_ptr<Tile>> tileTable;
    std::vector<std::vector<std::shared_ptr<Tile>>> map = {};
};

#endif