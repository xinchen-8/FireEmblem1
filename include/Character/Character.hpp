#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include "Integration.hpp"
#include "Item.hpp"
#include "Tile.hpp"
#include <cstdlib>
#include <queue>

class Item;
class HandHeldItem;

struct DijkstraNode {
    int cost;
    glm::ivec2 pos;
    std::queue<glm::ivec2> path;

    bool operator>(const DijkstraNode &other) const { return cost > other.cost; }
};

class Character : public CameraGameObject {
  public:
    Character(std::shared_ptr<MapManager> mm, std::vector<std::string> n_list, std::vector<std::string> g_list,
              std::shared_ptr<std::unordered_map<std::string, int>> wc, bool isPlayer = true);

    bool walkDirectly();
    void buildWalkPath(glm::ivec2 a_pos); // must in moveRange
    void refreshMoveRange(std::unordered_set<glm::ivec2> mask);
    void clearWalkPath();
    void resetRange(); // for PlayerManager::findCharacterAttackTarget
    void clearMoveRange() { moveRange.clear(); }
    void clearAttackRange() { attackRange.clear(); }

    void setAnimation();
    void setStatus(CharacterStatus status);
    void setForword(Forword forword) { this->forword = forword; }

    void setHeadshotAnimation(std::shared_ptr<Util::Animation> a_headshotAnimation) {
        headshotAnimation = a_headshotAnimation;
    }
    void setTileAnimation();

    void setHandHeldItemWithIndex(int index);
    void useVulnerary(int index);
    bool pushItem(std::shared_ptr<Item> item);
    void freshItem();
    void removeItem(int index);
    void deleteAllItems() { items = {}; }

    void setHP(int hp);
    void setAvoid(int avoid) { Avoid = avoid; }
    bool attacked(int power, int crt, int acc, bool isMagical = false); // return isAvoid;
    bool attack(std::shared_ptr<Character> target);

    std::shared_ptr<HandHeldItem> getCurrentHandHeldItem();

    bool isEnemy() const { return !isPlayer; }
    std::string getName() const { return name; }
    std::string getClassName() const { return className; }
    int getLevel() const { return Lv; }
    int getExp() const { return Ex; }
    int getHpLimit() const { return Hp_Limit; }
    int getCurHP() const { return Hp_Current; }
    int getStr() const { return Str; }
    int getSkl() const { return Skl; }
    int getWlv() const { return Wlv; }
    int getSpd() const { return Spd; }
    int getLck() const { return Lck; }
    int getDef() const { return Def; }
    int getRes() const { return Res; }
    int getMov() const { return Mov; }
    int getHPGR() const { return HPGR; }
    int getStrGR() const { return StrGR; }
    int getSklGR() const { return SklGR; }
    int getWlvGR() const { return WlvGR; }
    int getSpdGR() const { return SpdGR; }
    int getLckGR() const { return LckGR; }
    int getDefGR() const { return DefGR; }
    int getResGR() const { return ResGR; }
    int getOldHpLimit() const { return oldHpLimit; }
    int getOldStr() const { return oldStr; }
    int getOldSkl() const { return oldSkl; }
    int getOldWlv() const { return oldWlv; }
    int getOldSpd() const { return oldSpd; }
    int getOldLck() const { return oldLck; }
    int getOldDef() const { return oldDef; }
    int getOldRes() const { return oldRes; }
    int getAvoid() const { return Avoid; }
    int getHandHeldIndex() const { return handheld_index; }
    bool getVisible() const { return m_Visible; }
    CharacterStatus getStatus() const { return status; }
    std::vector<std::shared_ptr<Item>> getItems() { return items; }

    // Level up related methods
    void addExp(int exp);
    bool checkLevelUp();
    void levelUp();
    bool isLevelUp() const { return m_IsLevelUp; }
    void resetLevelUpFlag() { m_IsLevelUp = false; }

    std::unordered_map<glm::ivec2, int> getMoveRange() { return moveRange; }
    std::unordered_map<glm::ivec2, int> getAttackRange() { return attackRange; }
    std::queue<glm::ivec2> getWalkPath() { return walkPath; }
    std::unordered_map<std::string, int> getWalkCost() { return *walkCost; }

    void findMoveRange(int mov, glm::ivec2 a_pos, std::unordered_set<glm::ivec2> mask);
    void findHandHeldScope();
    void findAttackScope();
    void findAttackRange(); // for personal attack scope
    void setAttackRange(std::unordered_map<glm::ivec2, int> ar) { attackRange = ar; }
    std::vector<std::string> getUsableWeapon() { return usableHandHeldItem; }
    std::shared_ptr<Character> clone();
    // items
  protected:
    int gapOfAnimation = 3;
    // mask: enemy pos and locked tiles of map
    int handheld_index = 0;
    std::vector<std::shared_ptr<Item>> items;

  private:
    std::string name = "";
    std::string className = "";
    bool isPlayer = true;
    bool m_IsLevelUp = false; // Flag to indicate if character just leveled up

    Forword forword = Forword::Down;
    CharacterStatus status = CharacterStatus::Normal;
    std::queue<glm::ivec2> walkPath = {};
    std::vector<std::string> usableHandHeldItem = {};

    int Lv = 0;
    int Ex = 0;
    int Hp_Limit = 0;
    int Hp_Current = 0;
    int Str = 0;
    int Skl = 0;
    int Wlv = 0;
    int Spd = 0;
    int Lck = 0;
    int Def = 0;
    int Res = 0;
    int Mov = 0;

    int HPGR = 0;
    int StrGR = 0;
    int SklGR = 0;
    int WlvGR = 0;
    int SpdGR = 0;
    int LckGR = 0;
    int DefGR = 0;
    int ResGR = 0;

    int oldHpLimit = 0;
    int oldStr = 0;
    int oldSkl = 0;
    int oldWlv = 0;
    int oldSpd = 0;
    int oldLck = 0;
    int oldDef = 0;
    int oldRes = 0;

    int Avoid = 0;
    std::shared_ptr<MapManager> mapManager = nullptr;

    std::shared_ptr<Util::Animation> headshotAnimation = nullptr;
    std::shared_ptr<Util::Animation> standAnimation = nullptr;
    std::unordered_map<Forword, std::shared_ptr<Util::Animation>> walkAnimation = {};
    std::shared_ptr<Util::Animation> waitAnimation = nullptr;

    std::shared_ptr<std::unordered_map<std::string, int>> walkCost = nullptr;
    std::unordered_map<glm::ivec2, int> moveRange = {};
    std::unordered_map<glm::ivec2, int> attackRange = {};
};

#endif