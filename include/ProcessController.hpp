#ifndef PROCESSCONTROLLER_HPP
#define PROCESSCONTROLLER_HPP

#include "Character/CharacterManager.hpp"
#include "Selection.hpp"
#include "Tile.hpp"
#include "UserInterface/UIManager.hpp"

class ProcessController {
  public:
    ProcessController(std::shared_ptr<MapManager> mapManager, std::shared_ptr<PlayerManager> playerManager,
                      std::shared_ptr<EnemyManager> enemyManager, std::shared_ptr<Selection> selection,
                      std::shared_ptr<UIManager> uiManager);
    bool ReturnCase();
    void BackCase();
    void MovCase(glm::ivec2 mov);
    void MovCase(int listMov);

  private:
    void normalToMoving(std::shared_ptr<Character> &selectCharacter);
    void movingToSUI(std::shared_ptr<Character> &selectedCharacter);

    void SUItoOption();
    void TargetingToHUI(glm::ivec2 targetPos);
    void HealToNormal(std::shared_ptr<Character> &selectedCharacter, std::shared_ptr<Character> &selectPlayer);
    void ATKTargetingToATKWUI(glm::ivec2 targetPos);
    void ATKToNormal(std::shared_ptr<Character> &selectedCharacter, std::shared_ptr<Character> &selectEnemy);
    void IUIToNormal(std::shared_ptr<Character> &selectedCharacter);

    std::shared_ptr<MapManager> mapManager = nullptr;
    std::shared_ptr<PlayerManager> playerManager = nullptr;
    std::shared_ptr<EnemyManager> enemyManager = nullptr;
    std::shared_ptr<Selection> selection = nullptr;
    std::shared_ptr<UIManager> uiManager = nullptr;
};
#endif