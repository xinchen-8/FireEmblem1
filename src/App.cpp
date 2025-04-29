#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <iostream>

App::App(){
    playerManager->setCharaterManager(enemyManager);
    enemyManager->setCharaterManager(playerManager);
    selection->setAbsolutePos(playerManager->getCharacter("Marth")->getAbsolutePos());
    camera->resetCameraAbsolutePos();

    // playerManager->refreshAllCharacterMoveRange();
    // LOG_INFO("Player Move Manager Refresh Success.");
    // enemyManager->refreshAllCharacterMoveRange();
    // LOG_INFO("Enemy Move Manager Refresh Success.");
}

void App::Start() {
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;
}

void App::Update() {
    
    //end
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    // selection move
    if (((Util::Input::IsKeyPressed(Util::Keycode::UP) && delayKeyCounter <= delayKeyCheck) ||
        (Util::Input::IsKeyDown(Util::Keycode::UP))) && selection->moveJudge(Forword::Up, mapManager->getMapSize())){
            
        selection->moveDirectly({ 0, TILE_SIZE });
        delayKeyCounter = delayKeyLimit;

        uiManager->update();
    }
    
    if (((Util::Input::IsKeyPressed(Util::Keycode::DOWN) && delayKeyCounter <= delayKeyCheck) ||
        (Util::Input::IsKeyDown(Util::Keycode::DOWN))) && selection->moveJudge(Forword::Down, mapManager->getMapSize())){

        selection->moveDirectly({ 0, -TILE_SIZE });
        delayKeyCounter = delayKeyLimit;

        uiManager->update();
    }

    if (((Util::Input::IsKeyPressed(Util::Keycode::LEFT) && delayKeyCounter <= delayKeyCheck) ||
        (Util::Input::IsKeyDown(Util::Keycode::LEFT))) && selection->moveJudge(Forword::Left, mapManager->getMapSize())){

        selection->moveDirectly({ -TILE_SIZE, 0 });
        delayKeyCounter = delayKeyLimit;

        uiManager->update();
    }

    if (((Util::Input::IsKeyPressed(Util::Keycode::RIGHT) && delayKeyCounter <= delayKeyCheck) ||
        (Util::Input::IsKeyDown(Util::Keycode::RIGHT))) && selection->moveJudge(Forword::Right, mapManager->getMapSize())){
        
        selection->moveDirectly({ TILE_SIZE, 0 });
        delayKeyCounter = delayKeyLimit;

        uiManager->update();
    }

    //select character method
    if (Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
        LOG_INFO("Enter pressed");
        
        std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();
        std::shared_ptr<Character> selectCharacter = playerManager->getPosCharacter(selection->getAbsolutePos());
        std::shared_ptr<Character> selectEnemy = enemyManager->getPosCharacter(selection->getAbsolutePos());
        SelectionStatus status = selection->getStatus();

        //selectable => next is walk
        if(status == SelectionStatus::Normal && selectCharacter){
            
            selection->setSelectCharacter(selectCharacter);
            selection->setStatus(SelectionStatus::Moving);
            selection->setMoveLimit(playerManager->selectCharacter(selectCharacter));
        }
        //walkable => next is choose UI (test for attack)
        else if(status == SelectionStatus::Moving && 
            (!selectCharacter || selectCharacter == selectedCharacter)){
            
            selection->setStatus(SelectionStatus::Locked);
            playerManager->clearTips();
            selectedCharacter->buildWalkPath(selection->getAbsolutePos()); // trigger findCharacterAttackTarget when finished 
            
        }
        // UI mode is attack => next is select attacked target
        else if(status == SelectionStatus::Locked && selectedCharacter->getWalkPath().empty()){
            selection->setStatus(SelectionStatus::Targeting);
            playerManager->buildCharacterTips(selectedCharacter);
            if(selectedCharacter->getAttackRange().size()==0) LOG_ERROR("NO ENEMY...");
            selection->setMoveLimit(selectedCharacter->getAttackRange());
            selection->setAbsolutePos(selection->getLimitRange().begin()->first);
        }
        //attackable => attack
        else if(status == SelectionStatus::Targeting && selectEnemy){
            selectedCharacter->attack(selectEnemy);
        }
    }

    //go back
    if (Util::Input::IsKeyDown(Util::Keycode::BACKSPACE)) {
        LOG_INFO("Backspace pressed");

        SelectionStatus status = selection->getStatus();
        std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();
        
        //select attacked target => back to UI
        if(status == SelectionStatus::Targeting){
            selection->setStatus(SelectionStatus::Locked);
            selection->setAbsolutePos(selectedCharacter->getAbsolutePos());
            selectedCharacter->resetRange();
            playerManager->buildCharacterTips(selectedCharacter);
        }
        //select walk (UI) => back to select character
        if(status == SelectionStatus::Locked){
            if(!selectedCharacter) LOG_DEBUG("No character selected");
            
            selectedCharacter->setAbsolutePos(selection->getOriginalSelectionPos());
            selectedCharacter->setForword(Forword::Down);
            selectedCharacter->setStatus(CharacterStatus::Moving);
            selectedCharacter->clearWalkPath();
            
            selection->setMoveLimit(playerManager->selectCharacter(selectedCharacter));
            selection->setStatus(SelectionStatus::Moving);
            selection->setAbsolutePos(selection->getOriginalSelectionPos());
            camera->resetCameraAbsolutePos();
        }
        //select character => back to none
        else if(status == SelectionStatus::Moving){
            if(!selectedCharacter) LOG_DEBUG("No character selected");

            selectedCharacter->setAbsolutePos(selection->getOriginalSelectionPos());
            selectedCharacter->setStatus(CharacterStatus::Normal);
            selection->setStatus(SelectionStatus::Normal);
            selection->setAbsolutePos(selection->getOriginalSelectionPos());
            selection->setSelectCharacter(nullptr);
            playerManager->clearTips();
            camera->resetCameraAbsolutePos();
        }
    }

    //info UI
    if (Util::Input::IsKeyDown(Util::Keycode::F1)) {
        LOG_INFO("F1 pressed");

        uiManager->changeVisibleTileInfo();
    }
    //character info UI
    if (Util::Input::IsKeyDown(Util::Keycode::F2)) {
        LOG_INFO("F2 pressed");
        uiManager->changeVisibleCharacterInfo();
    }
    //player tip 
    if (Util::Input::IsKeyDown(Util::Keycode::F3)) {
        LOG_INFO("F3 pressed");

        playerManager->changeTipsVisible(selection->getSelectCharacter());
    }
    //all tip 
    // if (Util::Input::IsKeyDown(Util::Keycode::F3)) {
    //     LOG_INFO("F3 pressed");

    //     playerManager->buildCharacterTips();
    // }
    //update
    camera->update();
    playerManager->update();
    if (!--delayKeyCounter) delayKeyCounter = delayKeyLimit;
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}

