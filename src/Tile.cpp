#include "Tile.hpp"

namespace fs = std::filesystem;
namespace Tool {

std::shared_ptr<std::vector<std::vector<std::string>>> inputFile(std::string file) {
    std::ifstream input(file);
    if (!input.is_open()) {
        LOG_ERROR("can't open file: " + file);
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
        wordSpace.push_back(words);
    }
    return std::make_shared<std::vector<std::vector<std::string>>>(wordSpace);
}
} // namespace Tool

Tile::Tile(std::vector<std::string> t_list) : avoid(std::stoi(t_list[2])) {

    if (t_list.size() < 6)
        LOG_ERROR("The tile list is uncomplete!");

    std::vector<std::string> textures;
    std::filesystem::path tilePath(TILE_MAP);
    for (const auto &entry : fs::directory_iterator(tilePath)) {
        if (entry.is_regular_file()) {
            std::string fileName = entry.path().filename().string();

            if (fileName.rfind(t_list[1], 0) == 0) {
                std::cout << "add " << entry.path().string() << std::endl;
                textures.push_back(entry.path().string());
            }
        }
    }

    m_ZIndex = (t_list[4] != "0" || t_list[5] != "0") ? 1 : 0;
    SetPivot(glm::ivec2(TILE_PIXEL * std::stof(t_list[4]), TILE_PIXEL * std::stof(t_list[5])));

    m_Drawable = std::make_shared<Util::Animation>(textures, true, TILE_INTERVAL, true, 0);
    m_Transform.scale = glm::ivec2(TILE_SCALE, TILE_SCALE);

    char reg = t_list[1].back();
    if (reg == '_' || (reg >= '0' && reg <= '9'))
        t_list[1].pop_back();

    t_list[1][0] = std::toupper(t_list[1][0]);
    name = t_list[1];

    LOG_INFO("tile: " + t_list[0] + ". " + name + " Tile building success.");
}

Tile::Tile(std::string id, std::string img_path) {
    m_Drawable = std::make_shared<Util::Animation>(std::vector{img_path}, false, 0, false, 0);
    m_Transform.scale = glm::ivec2(TILE_SCALE, TILE_SCALE);
    LOG_INFO("tile: " + id + ". " + img_path + " Tile building success.");
}

Tile::Tile(const Tile &other, glm::ivec2 a_pos) {
    name = other.name;
    avoid = other.avoid;
    cost = other.cost;
    access = true;
    m_Pivot = other.m_Pivot;
    m_ZIndex = other.m_ZIndex;
    m_Drawable = std::make_shared<Util::Animation>(*std::dynamic_pointer_cast<Util::Animation>(other.m_Drawable));
    m_Transform = other.m_Transform;
    absolutePos = a_pos;
}

MapManager::MapManager(int level) : level(level) {
    buildTileTable();
    loadMap(level);
}

void MapManager::buildTileTable() {
    std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile(TILE_MAP "tiles.csv");
    if (!data)
        LOG_ERROR("Tile table building failed!");

    data->erase(data->begin());
    for (auto &e : *data)
        tileTable[e[0]] = std::make_shared<Tile>(e);
    LOG_INFO("Tile table building success.");
}

void MapManager::loadMap(int level) {
    this->level = level;
    std::shared_ptr<std::vector<std::vector<std::string>>> data =
        Tool::inputFile(DATA_MAP "FE1_map_ch" + std::to_string(level) + ".csv");
    if (!data)
        LOG_ERROR("Map loading failed!");

    int currentYIndex = 0;
    tileNum = {std::stoi((*data)[currentYIndex][0]), std::stoi((*data)[currentYIndex + 1][0])};

    map.clear();
    for (currentYIndex = 2; currentYIndex < tileNum.y + 2; currentYIndex++) {
        std::vector<std::string> e = (*data)[currentYIndex];
        std::vector<std::shared_ptr<Tile>> row;
        for (int i = 0; i < tileNum.x; i++) {
            std::string t = e[i];
            row.push_back(std::make_shared<Tile>(*tileTable[t], getTileAbsolutePos({i, currentYIndex - 2})));
        }
        map.push_back(row);
    }
    // end point
    endPosition = glm::ivec2({std::stoi((*data)[currentYIndex][0]) * TILE_SIZE,
                              (tileNum.y - std::stoi((*data)[currentYIndex][1]) - 1) * TILE_SIZE});
    // can't mov
    int numofPoint = std::stoi((*data)[++currentYIndex][0]);
    currentYIndex++;
    for (int i = 0; i < numofPoint; i++) {
        int regY = std::stoi((*data)[currentYIndex + i][1]);
        int regX = std::stoi((*data)[currentYIndex + i][0]);
        cantMovPosition.insert({regX * TILE_SIZE, (tileNum.y - regY - 1) * TILE_SIZE});
    }
    // mask texture
    currentYIndex += numofPoint;
    int numofMasks = std::stoi((*data)[currentYIndex][0]);
    currentYIndex++;
    for (int i = 0; i < numofMasks; i++) {
        int regY = std::stoi((*data)[currentYIndex + i][1]);
        int regX = std::stoi((*data)[currentYIndex + i][0]);
        map[regY][regX]->mask();
        LOG_INFO("Tile {" + std::to_string(regX) + ", " + std::to_string(regY) + "} mask success.");
    }

    LOG_INFO("Map loading success.");
}

void MapManager::startAnimations() {
    for (auto &row : map) {
        for (auto &tile : row) {
            tile->setStart();
        }
    }
}

void MapManager::stopAnimations() {
    for (auto &row : map) {
        for (auto &tile : row) {
            tile->setStop();
        }
    }
}

glm::ivec2 MapManager::getTileAbsolutePos(glm::ivec2 pos) {
    return {pos.x * TILE_SIZE, (tileNum.y - 1 - pos.y) * TILE_SIZE};
}

std::shared_ptr<Tile> MapManager::getPosTile(glm::ivec2 a_pos) {
    for (auto &row : map) {
        for (auto &e : row) {
            if (a_pos == e->getAbsolutePos())
                return e;
        }
    }
    return nullptr;
}

std::shared_ptr<Tile> MapManager::getTile(std::string id) { return tileTable[id]; }

std::vector<std::shared_ptr<CameraGameObject>> MapManager::getChildren() {
    std::vector<std::shared_ptr<CameraGameObject>> children;
    for (auto &row : map) {
        for (auto &tile : row) {
            children.push_back(tile);
        }
    }
    return children;
}
