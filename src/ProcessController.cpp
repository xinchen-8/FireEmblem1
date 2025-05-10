#include "ProcessController.hpp"

ProcessController::ProcessController(
    std::shared_ptr<MapManager> mapManager,
    std::shared_ptr<PlayerManager> playerManager,
    std::shared_ptr<EnemyManager> enemyManager,
    std::shared_ptr<Selection> selection,
    std::shared_ptr<UIManager> uiManager) : 
    mapManager(mapManager), playerManager(playerManager), enemyManager(enemyManager), 
    selection(selection), uiManager(uiManager){

}

void ProcessController::ReturnCase(){
    
    std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();
    std::shared_ptr<Character> selectMovableCharacter = playerManager->getPosMovableCharacter(selection->getAbsolutePos());
    std::shared_ptr<Character> selectLevelCharacter = playerManager->getPosLevelCharacter(selection->getAbsolutePos());
    std::shared_ptr<Character> selectEnemy = enemyManager->getPosLevelCharacter(selection->getAbsolutePos());
    SelectionStatus status = selection->getStatus();

    //normal -> moving
    if(status == SelectionStatus::Normal && selectMovableCharacter) 
        normalToMoving(selectMovableCharacter);
    //moving => SUI
    else if(status == SelectionStatus::Moving && (!selectLevelCharacter || selectMovableCharacter == selectedCharacter))
        movingToSUI(selectedCharacter);
    //SUI
    else if(status == SelectionStatus::Walking && selectedCharacter->getWalkPath().empty())
        SUItoOption();
    //act
    else if(status == SelectionStatus::SUI)
        uiManager->activeActUI();
    //attack targeting => WUI
    else if(status == SelectionStatus::AttackTargeting && selectEnemy)
        ATKTargetingToATKWUI(selectEnemy->getAbsolutePos());
    //WUI => attack then wait
    else if(status == SelectionStatus::AttackWUI)
        ATKToNormal(selectedCharacter, selectEnemy);
    //item UI => wait
    else if(status == SelectionStatus::ITEMIUI)
        IUIToNormal(selectedCharacter);
}

void ProcessController::BackCase(){
    
}

void ProcessController::MovCase(glm::ivec2 mov){
    if(selection->getStatus()==SelectionStatus::SUI) return;
    selection->moveDirectly(mov);
    uiManager->load();
    uiManager->update();
}

void ProcessController::MovCase(int listMov){
    uiManager->updateActUI(listMov);
    uiManager->updateWeaponUI(listMov);
    //other ui...
}

void ProcessController::normalToMoving(std::shared_ptr<Character> &selectCharacter){
    selection->setSelectCharacter(selectCharacter);
    selection->setStatus(SelectionStatus::Moving);
    selection->setMoveLimit(playerManager->selectCharacter(selectCharacter));
}


void ProcessController::movingToSUI(std::shared_ptr<Character> &selectedCharacter){
    selection->setStatus(SelectionStatus::Walking);
    playerManager->clearTips();
    selectedCharacter->buildWalkPath(selection->getAbsolutePos()); // trigger findCharacterAttackTarget when finished 
}

void ProcessController::SUItoOption(){
    uiManager->activeActUI();
}

void ProcessController::ATKTargetingToATKWUI(glm::ivec2 targetPos){
    selection->setStatus(SelectionStatus::AttackWUI);
    uiManager->loadWeaponUI(targetPos);
}

void ProcessController::ATKToNormal(
    std::shared_ptr<Character> &selectedCharacter, 
    std::shared_ptr<Character> &selectEnemy){

    uiManager->actWeaponUI();
    selectedCharacter->attack(selectEnemy);
    selectedCharacter->setStatus(CharacterStatus::Waiting);
    playerManager->removeUnwaitingCharacter(selectedCharacter);
    playerManager->clearTips();
    selection->setStatus(SelectionStatus::Normal);

    uiManager->load();
    uiManager->update();
}

void ProcessController::IUIToNormal(std::shared_ptr<Character> &selectedCharacter){
    selectedCharacter->setStatus(CharacterStatus::Waiting);
    playerManager->removeUnwaitingCharacter(selectedCharacter);
}