#ifndef TILE_HPP
#define TILE_HPP

#include "Data.hpp"
#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace Tool {
std::shared_ptr<std::vector<std::vector<std::string>>> inputFile(std::string file);
}

class Tile : public CameraGameObject {

  public:
    Tile(std::vector<std::string> t_list);
    // for UI
    Tile(std::string id, std::string img_path);
    // for map
    Tile(const Tile &other, glm::ivec2 a_pos);

    void setRelativePos(glm::ivec2 r_pos) { m_Transform.translation = r_pos; }
    void setAnimation(std::shared_ptr<Util::Animation> a_animation) { m_Drawable = a_animation; }
    void setStop() { std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->Pause(); }
    void setStart() { std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->Play(); }
    void setVisible(bool visible) { m_Visible = visible; }
    void mask() {
        access = false;
        m_Visible = false;
    }

    glm::ivec2 getRelativePos() const { return m_Transform.translation; }
    std::string getName() const { return name; }
    bool getMask() const { return access; }
    int getAvoid() const { return avoid; }

  private:
    std::string name = "";
    int avoid = 0;
    int cost = 0;
    bool access = true;
};

class MapManager {
  public:
    MapManager(int level);
    void buildTileTable();
    void loadMap(int level);

    void startAnimations();
    void stopAnimations();
    void setAbsolutePos(glm::ivec2 a_pos) { absolutePos = a_pos; }

    bool nextLevel(glm::ivec2 marthPosition) { return marthPosition == endPosition; }

    glm::ivec2 getTileAbsolutePos(glm::ivec2 pos);
    glm::ivec2 getMapSize() const { return glm::ivec2(map[0].size() * TILE_SIZE, map.size() * TILE_SIZE); }
    glm::ivec2 getMapTileSize() const { return glm::ivec2(map[0].size(), map.size()); }
    glm::ivec2 getAbsolutePos() const { return absolutePos; }
    std::unordered_set<glm::ivec2> getAbsoluteCantMovPosition() { return cantMovPosition; }
    bool isGoldCoin(glm::ivec2 pos) { return goldCoin.count(pos) && goldCoin[pos] > 0; }
    int getGoldCoin(glm::ivec2 pos);
    bool isArmory(glm::ivec2 pos);

    int getLevel() const { return level; }
    std::shared_ptr<Tile> getPosTile(glm::ivec2 a_pos);
    std::shared_ptr<Tile> getTile(std::string id);
    std::vector<std::shared_ptr<CameraGameObject>> getChildren();

  private:
    int level = 0;
    glm::ivec2 tileNum = {0, 0};
    glm::ivec2 endPosition = {0, 0};
    std::unordered_set<glm::ivec2> cantMovPosition = {};
    std::unordered_map<glm::ivec2, int> goldCoin = {};
    std::vector<glm::ivec2> armory = {};

    glm::ivec2 absolutePos = {0, 0}; // main point of camera is down left corner of map
    std::unordered_map<std::string, std::shared_ptr<Tile>> tileTable;
    std::vector<std::vector<std::shared_ptr<Tile>>> map = {};
};

#endif