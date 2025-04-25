#ifndef CHARACTERMANAGER_HPP
#define CHARACTERMANAGER_HPP

#include "Integration.hpp"
#include "Character.hpp"

class CharacterManager{
public:
    //init
    CharacterManager(std::shared_ptr<MapManager> mm);
    virtual void loadCharacter() = 0;
    virtual void setInitialLevel(int level) = 0;

    // void refreshAllCharacterMoveRange();
    // void buildCharacterTips(); //Overload
    void clearTips();

    virtual void update();

    void setCharaterManager(std::weak_ptr<CharacterManager> cm){ characterManager = cm;}

    std::shared_ptr<Character> getPosCharacter(glm::ivec2 a_pos);
    std::unordered_set<glm::ivec2> getCharacterPos();
    std::shared_ptr<Tile> getTipTile(glm::ivec2 a_pos);
    std::vector<std::shared_ptr<CameraGameObject>> getChildren();

protected:
    bool tipsVisible = false;

    std::unordered_map<std::string, std::shared_ptr<std::unordered_map<std::string, int>>> costTable = {};
    std::vector<std::shared_ptr<Character>> characters = {};

    std::shared_ptr<MapManager> mapManager = nullptr;
    std::weak_ptr<CharacterManager> characterManager;
    std::vector<std::vector<std::shared_ptr<Tile>>> tips = {};
};

class PlayerManager : public CharacterManager{
public:
    // using CharacterManager::buildCharacterTips;
    PlayerManager(std::shared_ptr<MapManager> mm);
    void loadCharacter() override;
    void setInitialLevel(int level) override;
    
    void update() override;

    void changeTipsVisible(std::shared_ptr<Character> character = nullptr);
    void buildCharacterTips(std::shared_ptr<Character> character);
    
    std::unordered_map<glm::ivec2, int> selectCharacter(std::shared_ptr<Character> character);
    void findCharacterAttackTarget(std::shared_ptr<Character> character);
    std::shared_ptr<Character> getCharacter(std::string id);
};

class EnemyManager : public CharacterManager{
public:
    // using CharacterManager::buildCharacterTips;
    EnemyManager(std::shared_ptr<MapManager> mm);
    void loadCharacter() override;
    void setInitialLevel(int level) override;
};

#endif