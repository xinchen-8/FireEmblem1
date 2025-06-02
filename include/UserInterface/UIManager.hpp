#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP
#include "UserInterface/BattleUI.hpp"
#include "UserInterface/InfoUI.hpp"
#include "UserInterface/LoadUI.hpp"
#include "UserInterface/SelectedUI.hpp"
#include "UserInterface/UserInterface.hpp"

class UIManager {
  public:
    UIManager(std::shared_ptr<Selection> s, std::shared_ptr<MapManager> tm, std::shared_ptr<PlayerManager> pm,
              std::shared_ptr<EnemyManager> em);
    void load();
    void update();

    bool closeLoadUI();

    void loadActUI();
    void updateActUI(int listMov) { selectedAct->update(listMov); }
    bool activeActUI(); // return isNextLevel;

    void loadWeaponUI(glm::ivec2 targetPos, bool isHealSpecialCase);
    void updateWeaponUI(int listMov) { selectedWeapon->update(listMov); }
    void actWeaponUI();

    void loadItemUI();
    void updateItemUI(int listMov) { selectedItem->update(listMov); }
    void actItemUI();

    void loadBattleUI(std::shared_ptr<Character> attacker, std::shared_ptr<Character> attacked);
    bool updateBattleUI();
    // load as act

    void changeVisibleTileInfo();
    // void changeVisibleCharacterInfo();
    void changeVisibleCharacterInfoFull();

    std::vector<std::shared_ptr<Util::GameObject>> getChildren();

  private:
    std::shared_ptr<MapManager> mapManager = nullptr;
    std::shared_ptr<Selection> selection = nullptr;
    std::shared_ptr<PlayerManager> playerManager = nullptr;
    std::shared_ptr<EnemyManager> enemyManager = nullptr;

    std::vector<std::shared_ptr<Tile>> tiles = {};
    std::shared_ptr<LoadUI> loadLevel = nullptr;
    std::shared_ptr<TileInfoUI> tileInfo = nullptr;
    // std::shared_ptr<CharacterInfoUI> characterInfo = nullptr;
    std::shared_ptr<CharacterInfoUIFull> characterInfoFull = nullptr;
    std::shared_ptr<ActUI> selectedAct = nullptr;
    std::shared_ptr<WeaponUI> selectedWeapon = nullptr;
    std::shared_ptr<ItemUI> selectedItem = nullptr;
    std::shared_ptr<BattleUI> battle = nullptr;
};

#endif