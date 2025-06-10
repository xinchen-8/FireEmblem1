#ifndef PROCESSCONTROLLER_HPP
#define PROCESSCONTROLLER_HPP

#include "Camera.hpp"
#include "Character/CharacterManager.hpp"
#include "Selection.hpp"
#include "Tile.hpp"
#include "UserInterface/UIManager.hpp"

struct A_Node {
    glm::ivec2 pos;
    int gCost, hCost, fCost;

    bool operator>(const A_Node &other) const { return fCost > other.fCost; }
};

class ProcessController {
  public:
    ProcessController(std::shared_ptr<MapManager> mapManager, std::shared_ptr<PlayerManager> playerManager,
                      std::shared_ptr<EnemyManager> enemyManager, std::shared_ptr<Selection> selection,
                      std::shared_ptr<UIManager> uiManager, std::shared_ptr<Camera> camera);
    bool ReturnCase();
    void SpaceCase();
    void BackCase();
    void MovCase(glm::ivec2 mov);
    void MovCase(int listMov);

    bool enemyTurn(bool accessInput);

  private:
    void normalToMoving(std::shared_ptr<Character> &selectCharacter);
    void movingToSUI(std::shared_ptr<Character> &selectedCharacter);

    // enter
    void SUItoOption();
    void TargetingToHUI(glm::ivec2 targetPos);
    void HealToNormal(std::shared_ptr<Character> &selectedCharacter, std::shared_ptr<Character> &selectPlayer);
    void ATKTargetingToATKWUI(glm::ivec2 targetPos);
    void ATKToNormal(std::shared_ptr<Character> &selectedCharacter, std::shared_ptr<Character> &selectEnemy);
    void IUIToSUI(std::shared_ptr<Character> &selectedCharacter);
    void ShopUItoShopUI();

    int heuristic(const glm::ivec2 &a, const glm::ivec2 &b);
    std::unordered_map<glm::ivec2, int> findBestPathToMarth(std::shared_ptr<Character> &enemy);

    std::shared_ptr<MapManager> mapManager = nullptr;
    std::shared_ptr<PlayerManager> playerManager = nullptr;
    std::shared_ptr<EnemyManager> enemyManager = nullptr;
    std::shared_ptr<Selection> selection = nullptr;
    std::shared_ptr<UIManager> uiManager = nullptr;
    std::shared_ptr<Camera> camera = nullptr;
};
#endif