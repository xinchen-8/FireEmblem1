#ifndef MAP_HPP
#define MAP_HPP

#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include <fstream>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

#define TILE_SCALE 3
#define TILE_SIZE 16*TILE_SCALE

namespace Tool {
    enum class Accessable{
        Walkable,
        Swimmable,
        Flyable
    };
 
    std::shared_ptr<std::vector<std::vector<std::string>>> inputFile(std::string file);
}

class Tile : public Util::GameObject{
public:
    Tile(std::vector<std::string> t_list);
    Tile(const Tile& other, glm::vec2 pos);
    bool Access(Tool::Accessable c_accessable);
    void setStop(){ std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->Pause(); }
    void setStart(){ std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->Play(); }
    void setPos(glm::vec2 pos){ m_Transform.translation = pos; }
    glm::vec2 getPos() const{ return m_Transform.translation; }
    // int getCurrentFrame() const { return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetCurrentFrameIndex(); }// for testing
private:
    std::string name = "";
    int avoid = 0;
    int cost = 0;
    Tool::Accessable access = Tool::Accessable::Walkable;
};

class Map{
public:
    Map(int level);
    void buildTileTable();
    void loadMap(int level);
    void startAnimations();
    void stopAnimations();
    Tile getTile(glm::vec2 pos);    //coordinate: start from the upper left corner
    std::vector<std::shared_ptr<Util::GameObject>> getChildren();
    // void checkTileFrameWork();
    int getCurrentMap() const { return level; }

private:
    int level = 0;
    std::unordered_map<std::string, std::shared_ptr<Tile>> tileTable;
    std::shared_ptr<std::vector<std::vector<Tile>>> map = nullptr;
};
#endif