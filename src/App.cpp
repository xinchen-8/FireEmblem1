#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <iostream>

App::App(){
    
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
        (Util::Input::IsKeyDown(Util::Keycode::UP))) &&
        selection->getAbsolutePos().y < mapManager->getMapSize().y - TILE_SIZE) {

        selection->moveDirectly({ 0, TILE_SIZE });
        delayKeyCounter = delayKeyLimit;

        uiManager->update();
    }
    
    if (((Util::Input::IsKeyPressed(Util::Keycode::DOWN) && delayKeyCounter <= delayKeyCheck) ||
        (Util::Input::IsKeyDown(Util::Keycode::DOWN))) &&
        selection->getAbsolutePos().y > 0){

        selection->moveDirectly({ 0, -TILE_SIZE });
        delayKeyCounter = delayKeyLimit;

        uiManager->update();
    }

    if (((Util::Input::IsKeyPressed(Util::Keycode::LEFT) && delayKeyCounter <= delayKeyCheck) ||
        (Util::Input::IsKeyDown(Util::Keycode::LEFT))) &&
        selection->getAbsolutePos().x > 0){

        selection->moveDirectly({ -TILE_SIZE, 0 });
        delayKeyCounter = delayKeyLimit;

        uiManager->update();
    }

    if (((Util::Input::IsKeyPressed(Util::Keycode::RIGHT) && delayKeyCounter <= delayKeyCheck) ||
        (Util::Input::IsKeyDown(Util::Keycode::RIGHT))) && 
        selection->getAbsolutePos().x < mapManager->getMapSize().x - TILE_SIZE){
        
        selection->moveDirectly({ TILE_SIZE, 0 });
        delayKeyCounter = delayKeyLimit;

        uiManager->update();
    }

    if (Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
        LOG_INFO("Enter pressed");
        std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();
        std::shared_ptr<Character> selectCharacter = characterManager->getPosCharacter(selection->getAbsolutePos());
        
        if(selection->getStatus() == SelectionStatus::Moving && (!selectCharacter || selectCharacter == selectedCharacter)){
            selection->setStatus(SelectionStatus::Waiting);
            characterManager->clearTips();
            selectedCharacter->moveDirectly(selection->getAbsolutePos());
            delayCharacterWalkCounter = delayCharacterWalkLimit; //for walking
            // selectCharacter->setStatus(CharacterStatus::Waiting); //not yet (fot test)
            
            //after UI (another function
            selection->setSelectCharacter(nullptr);
            selection->setStatus(SelectionStatus::Normal);
        }
        else if(selection->getStatus() == SelectionStatus::Normal && selectCharacter){
            selection->setSelectCharacter(selectCharacter);
            selection->setStatus(SelectionStatus::Moving);
            selection->setMoveLimit(characterManager->selectCharacter(selectCharacter));
        }
    }

    //info UI
    if (Util::Input::IsKeyDown(Util::Keycode::F1)) {
        uiManager->changeVisibleTileInfo();
    }
    //tip 
    if (Util::Input::IsKeyDown(Util::Keycode::F2)) {
        characterManager->changeTipsVisible(selection->getSelectCharacter());
    }
    //update
    camera->update();
    if (!--delayKeyCounter) delayKeyCounter = delayKeyLimit;
    if (!--delayCharacterWalkCounter){
        delayCharacterWalkCounter = delayCharacterWalkLimit;
        characterManager->update();
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}

