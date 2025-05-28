#ifndef BATTLEUI_HPP
#define BATTLEUI_HPP

#include "UserInterface/UserInterface.hpp"

enum class followType { Attacker, Attacked };

class BattleUI : public UserInterface {
  public:
    BattleUI(std::vector<std::shared_ptr<Tile>> &tiles);
    void load(std::shared_ptr<Character> attacker, std::shared_ptr<Character> attacked);
    void update() override;
    void refreshHpPoint();
    void setVisible(bool visible) override;
    std::vector<std::shared_ptr<Util::GameObject>> getChildren() override;
    bool isFinish();

  private:
    bool finishTrigger = false;
    bool canCounterAttack = false;
    bool canFollowUpAttack = false;
    followType followUpType = followType::Attacker;

    std::shared_ptr<Character> attackerCharacter = nullptr;
    std::shared_ptr<Util::GameObject> attackerGO = std::make_shared<Util::GameObject>();
    std::shared_ptr<Util::GameObject> attackerHpPoint = std::make_shared<Util::GameObject>();
    std::shared_ptr<Util::GameObject> attackerDetailList = std::make_shared<Util::GameObject>();
    std::shared_ptr<Util::GameObject> attackerDetailPoint = std::make_shared<Util::GameObject>();
    std::string attackerImg = "";

    std::shared_ptr<Character> attackedCharacter = nullptr;
    std::shared_ptr<Util::GameObject> attackedGO = std::make_shared<Util::GameObject>();
    std::shared_ptr<Util::GameObject> attackedHpPoint = std::make_shared<Util::GameObject>();
    std::shared_ptr<Util::GameObject> attackedDetailList = std::make_shared<Util::GameObject>();
    std::shared_ptr<Util::GameObject> attackedDetailPoint = std::make_shared<Util::GameObject>();
    std::string attackedImg = "";
};

#endif