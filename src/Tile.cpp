#include "Tile.hpp"

namespace fs = std::filesystem;
namespace Tool{
    
    std::shared_ptr<std::vector<std::vector<std::string>>> inputFile(std::string file){
        std::ifstream input(file);
        if (!input.is_open()) {
            LOG_ERROR("can't open file: " + file );
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
    
            //for (const auto& w : words) {
            //    std::cout << w << ".";
            //}
            wordSpace.push_back(words);
            //std::cout<<std::endl;
        }
        return std::make_shared<std::vector<std::vector<std::string>>>(wordSpace);
    }
}


Tile::Tile(std::vector<std::string> t_list) : 
avoid(std::stoi(t_list[2])), access(static_cast<Accessable>(std::stoi(t_list[3]))){

    if(t_list.size()<6) LOG_ERROR("The tile list is uncomplete!");
    
    std::vector<std::string> textures;
    std::filesystem::path tilePath(ASSETS TILES_FOLDER);
    for (const auto& entry : fs::directory_iterator(tilePath)) {
        if (entry.is_regular_file()) {
            std::string fileName = entry.path().filename().string();
            
            if (fileName.rfind(t_list[1], 0) == 0) {
                std::cout << "add " << entry.path().string() << std::endl;
                textures.push_back(entry.path().string());
            }
        }
    }

    m_ZIndex = (t_list[4]!="0" || t_list[5] != "0") ? 1 : 0;
    SetPivot(glm::vec2(TILE_PIXEL * std::stof(t_list[4]), TILE_PIXEL * std::stof(t_list[5])));

    m_Drawable = std::make_shared<Util::Animation>(textures, true, TILE_INTERVAL, true, 0);
    m_Transform.scale = glm::vec2(TILE_SCALE, TILE_SCALE);

    char reg = t_list[1].back();
    if(reg == '_' || (reg>='0'&&reg<='9')) t_list[1].pop_back();
    name = t_list[1];
    
    LOG_INFO("tile: " + t_list[0] +". " + name + " Tile building success.");
}

Tile::Tile(std::string id, std::string img_path) {
    m_Drawable = std::make_shared <Util::Animation>(std::vector{img_path}, false, 0, false, 0);
    m_Transform.scale = glm::vec2(TILE_SCALE, TILE_SCALE);
    LOG_INFO("tile: "+ id + ". " + img_path + " Tile building success.");
}

Tile::Tile(const Tile& other, glm::vec2 a_pos){
    name = other.name;
    avoid = other.avoid;
    cost = other.cost;
    access = other.access;
    m_Pivot = other.m_Pivot;
    m_ZIndex = other.m_ZIndex;
    m_Drawable = std::make_shared<Util::Animation>(
        *std::dynamic_pointer_cast<Util::Animation>(other.m_Drawable)
    );
    m_Transform = other.m_Transform;
    absolutePos = a_pos;
}

bool Tile::Access(Accessable c_accessable) {
    if (static_cast<int>(c_accessable) > static_cast<int>(access)) return true;
    return false;
}

bool Tile::mask(Accessable n_access) {
    if (access <= n_access) {
        access = n_access;
        m_Drawable = nullptr;
        return true;
    }
    return false;
}

void TileManager::maskTile(glm::vec2 pos, Accessable access) {
    if(map[pos.y][pos.x]->mask(access))
        LOG_INFO("Tile {"+ std::to_string(pos.x) + ", " + std::to_string(pos.y) + "} mask success.");
    else
        LOG_ERROR("Tile {" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + "} mask not success!");
}

TileManager::TileManager(int level) : level(level){
    buildTileTable();
    loadMap(level);
}

void TileManager::buildTileTable(){
    std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile(ASSETS TILES_FOLDER "tiles.csv");
    if (!data) LOG_ERROR("Tile table building failed!");

    data->erase(data->begin());
    for(auto &e: *data) tileTable[e[0]] = std::make_shared<Tile>(e);
    LOG_INFO("Tile table building success.");
}

void TileManager::loadMap(int level){
    level = this->level;
    std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile(ASSETS MAP_FOLDER "FE1_map_ch" + std::to_string(level) + ".csv");
    if(!data) LOG_ERROR("Map loading failed!");

    map = {};
    tileNum = {std::stoi((*data)[0][0]), std::stoi((*data)[1][0])};

    for(size_t j=2; j < tileNum.y+2; j++){
        std::vector<std::string> e = (*data)[j];
        std::vector<std::shared_ptr<Tile>> row;

        for(size_t i=0; i<tileNum.x; i++){
            std::string t = e[i];
            row.push_back(std::make_shared<Tile>(*tileTable[t], getTileAbsolutePos({ i, j-2 })));
        }
        map.push_back(row);
    }

    //mask
    int maskNum = stoi((*data)[tileNum.y + 2][0]);
    for (int i = tileNum.y + 3; i < tileNum.y + 3 + maskNum; i++) {
        maskTile({ stoi((*data)[i][0]), stoi((*data)[i][1]) }, static_cast<Accessable>(stoi((*data)[i][2])));
    }

    LOG_INFO("Map loading success.");
}

void TileManager::startAnimations(){
    for(auto &row: map){
        for(auto &tile: row){
            tile->setStart();
        }
    }
}

void TileManager::stopAnimations(){
    for(auto &row: map){
        for(auto &tile: row){
            tile->setStop();
        }
    }
}

glm::vec2 TileManager::getTileAbsolutePos(glm::vec2 pos) {
    return {pos.x * TILE_SIZE, (tileNum.y - 1 - pos.y) * TILE_SIZE };
}

std::shared_ptr<Tile> TileManager::getPosTile(glm::vec2 a_pos) {
    for (auto& row : map) {
        for (auto& e : row) {
            if (a_pos == e->getAbsolutePos())
                return e;
        }
    }
    return nullptr;
}

std::shared_ptr<Tile> TileManager::getTile(std::string id) {
    return tileTable[id];
}

std::vector<std::shared_ptr<CameraGameObject>> TileManager::getChildren(){
    std::vector<std::shared_ptr<CameraGameObject>> children;
    for(auto &row: map){
        for(auto &tile: row){
            children.push_back(tile);
        }
    }
    return children;
}