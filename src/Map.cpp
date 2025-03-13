#include "Map.hpp"
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;
namespace Tool{
    
    std::shared_ptr<std::vector<std::vector<std::string>>> inputFile(std::string file){
        std::ifstream input(file);
        if (!input.is_open()) {
            std::cerr << "Can't open " << file << std::endl;
            return nullptr;
        }
    
        std::vector<std::vector<std::string>> wordSpace;
        std::string line;
        while (std::getline(input, line)) {
    
            std::vector<std::string> words;
            size_t pos = 0;
            std::string token;
            std::string delimiter = ",";
    
            while ((pos = line.find(delimiter)) != std::string::npos) {
                token = line.substr(0, pos);
                words.push_back(token);
                line.erase(0, pos + delimiter.length()); 
            }
            words.push_back(line);
    
            for (const auto& w : words) {
                std::cout << w << ".";
            }
            wordSpace.push_back(words);
            std::cout<<std::endl;
        }
        return std::make_shared<std::vector<std::vector<std::string>>>(wordSpace);
    }
}

Tile::Tile(std::vector<std::string> t_list) : 
avoid(std::stoi(t_list[2])), cost(std::stoi(t_list[3])), access(static_cast<Tool::Accessable>(std::stoi(t_list[4]))){

    if(t_list.size()<7) std::cerr<<"The tile list is uncomplete!"<<std::endl;
    
    std::vector<std::string> textures;
    std::filesystem::path tilePath("./assets/tiles/");
    for (const auto& entry : fs::directory_iterator(tilePath)) {
        if (entry.is_regular_file()) {
            std::string fileName = entry.path().filename().string();
            
            if (fileName.rfind(t_list[1], 0) == 0) {
                std::cout << "add " << entry.path().string() << std::endl;
                textures.push_back(entry.path().string());
            }
        }
    }
    for (const auto& t : textures) {
        std::cout << t << ".";
    }
    std::cout << std::endl;
    if( t_list[5]!="0" ){
        m_ZIndex = 1;
        SetPivot(glm::vec2(-16, 16*0.5));
        // SetPivot(glm::vec2(-TILE_SIZE*std::stof(t_list[5]), TILE_SIZE*std::stof(t_list[6])));
    }
    m_Drawable = std::make_shared<Util::Animation>(textures, false, 750, true, 0);
    m_Transform.scale = glm::vec2(TILE_SCALE, TILE_SCALE);

    char reg = t_list[1].back();
    if(reg == '_' || (reg>='0'&&reg<='9')) t_list[1].pop_back();
    name = t_list[1];

    std::cout << t_list[0] << " " <<name << " Tile building success." << std::endl;
}

Tile::Tile(const Tile& other, glm::vec2 pos){
    name = other.name;
    avoid = other.avoid;
    cost = other.cost;
    access = other.access;
    m_Pivot = other.m_Pivot;
    m_ZIndex = other.m_ZIndex;
    m_Drawable = std::make_shared<Util::Animation>(*std::dynamic_pointer_cast<Util::Animation>(other.m_Drawable));
    m_Transform = other.m_Transform;
    m_Transform.translation = pos;
}

bool Tile::Access(Tool::Accessable c_accessable){
    if (static_cast<int>(c_accessable) > static_cast<int>(access)) return true;
    return false;
}

Map::Map(int level) : level(level){
    buildTileTable();
    loadMap(level);
}

void Map::buildTileTable(){
    std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile("./assets/tiles/tiles.csv");
    if(!data) std::cerr<<"Tile table building failed!"<<std::endl;

    data->erase(data->begin());
    for(auto &e: *data) tileTable[e[0]] = std::make_shared<Tile>(e);
    std::cout<<"Tile table building success."<<std::endl;
}

void Map::loadMap(int level){
    std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile("./assets/maps/FE1_map_ch" + std::to_string(level) + ".csv");
    if(!data) std::cerr<<"Map loading failed!"<<std::endl;

    map = std::make_shared<std::vector<std::vector<Tile>>>();
    for(size_t j=0; j < data->size(); j++){
        std::vector<std::string> e = (*data)[j];
        std::vector<Tile> row;

        for(size_t i=0; i<e.size(); i++){
            std::string t = e[i];
            Tile newTile(*tileTable[t], glm::vec2((-18+int(i))*TILE_SIZE, (10-int(j))*TILE_SIZE));
            row.push_back(newTile);
        }
        map->push_back(row);
    }
    std::cout<<"Map loading success."<<std::endl;
}

void Map::startAnimations(){
    // checkTileFrameWork();
    for(auto &row: *map){
        for(auto &tile: row){
            tile.setStart();
        }
    }
}

void Map::stopAnimations(){
    for(auto &row: *map){
        for(auto &tile: row){
            tile.setStop();
        }
    }
}

// void Map::checkTileFrameWork(){
//     for(auto &row: *map){
//         for(auto &tile: row){
//             std::cout<<tile.getCurrentFrame()<<".";
//         }
//         std::cout<<std::endl;
//     }
//     std::cout<<std::endl;
// }

Tile Map::getTile(glm::vec2 pos){
    return (*map)[int(pos.y)][int(pos.x)];
}

std::vector<std::shared_ptr<Util::GameObject>> Map::getChildren(){
    std::vector<std::shared_ptr<Util::GameObject>> children;
    for(auto &row: *map){
        for(auto &tile: row){
            children.push_back(std::make_shared<Util::GameObject>(tile));
        }
    }
    return children;
}