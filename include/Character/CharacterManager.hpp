#ifndef CHARACTERMANAGER_HPP
#define CHARACTERMANAGER_HPP

#include "Character/Character.hpp"
#include "Integration.hpp"

class CharacterManager {
  public:
    // init
    CharacterManager(std::shared_ptr<MapManager> mm);
    void loadCharacter();
    void setInitialLevel(int level);
    void characterIsDead(std::shared_ptr<Character> c);
    void removeUnwaitingCharacter(std::shared_ptr<Character> c);
    void reloadUnwaitingCharacter();
    void clearTips();

    virtual bool update() = 0;

    void setCharaterManager(std::weak_ptr<CharacterManager> cm) { characterManager = cm; }
    bool isNoMovableCharacter();

    std::shared_ptr<Character> getCharacter(std::string id);
    std::shared_ptr<Character> getPosMovableCharacter(glm::ivec2 a_pos);
    std::shared_ptr<Character> getPosLevelCharacter(glm::ivec2 a_pos);
    std::unordered_set<glm::ivec2> getCharacterPos();

    std::shared_ptr<Tile> getTipTile(glm::ivec2 a_pos);
    std::vector<std::shared_ptr<Character>> getCurrentLevelCharacters() { return currentLevelCharacters; }
    std::vector<std::shared_ptr<Character>> getCurrentUnwaitedCharacters() { return currentUnwaitedCharacters; }
    std::vector<std::shared_ptr<CameraGameObject>> getChildren();

  protected:
    bool isEnemy = false;
    bool tipsVisible = true;

    std::unordered_map<std::string, std::shared_ptr<std::unordered_map<std::string, int>>> costTable = {};
    std::vector<std::shared_ptr<Character>> characters = {};
    std::vector<std::shared_ptr<Character>> currentLevelCharacters = {};
    std::vector<std::shared_ptr<Character>> currentUnwaitedCharacters = {};

    std::shared_ptr<MapManager> mapManager = nullptr;
    std::weak_ptr<CharacterManager> characterManager;
    std::vector<std::vector<std::shared_ptr<Tile>>> tips = {};
};

class PlayerManager : public CharacterManager {
  public:
    // using CharacterManager::buildCharacterTips;
    PlayerManager(std::shared_ptr<MapManager> mm);
    bool update() override; // return isTrigger
    void changeTipsVisible(std::shared_ptr<Character> character = nullptr);
    void buildCharacterTips(std::shared_ptr<Character> character);

    std::unordered_map<glm::ivec2, int> selectCharacter(std::shared_ptr<Character> character);
    void findCharacterAttackTarget(std::shared_ptr<Character> character);

    void addMoney(int num) { money += num; }
    int getMoney() { return money; }
    // special case
    bool isNearEnemy(std::string name1, std::string name2);
    void WryTrigger();
    void TalkTrigger(std::string name);

  private:
    int money = 0;
};

class EnemyManager : public CharacterManager {
  public:
    // using CharacterManager::buildCharacterTips;
    EnemyManager(std::shared_ptr<MapManager> mm);
    bool update() override;           // return isTrigger
    bool enemyTurn(bool accessInput); // return finished
};

#endif