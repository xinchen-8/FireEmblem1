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
    if (((Util::Input::IsKeyPressed(Util::Keycode::UP) && delayCounter <= delayCheck) ||
        (Util::Input::IsKeyDown(Util::Keycode::UP))) &&
        selection->getAbsolutePos().y < mapManager->getMapSize().y - TILE_SIZE) {

        selection->moveDirectly({ 0, TILE_SIZE });
        delayCounter = delayLimit;

        uiManager->update();
    }
    
    if (((Util::Input::IsKeyPressed(Util::Keycode::DOWN) && delayCounter <= delayCheck) ||
        (Util::Input::IsKeyDown(Util::Keycode::DOWN))) &&
        selection->getAbsolutePos().y > 0){

        selection->moveDirectly({ 0, -TILE_SIZE });
        delayCounter = delayLimit;

        uiManager->update();
    }

    if (((Util::Input::IsKeyPressed(Util::Keycode::LEFT) && delayCounter <= delayCheck) ||
        (Util::Input::IsKeyDown(Util::Keycode::LEFT))) &&
        selection->getAbsolutePos().x > 0){

        selection->moveDirectly({ -TILE_SIZE, 0 });
        delayCounter = delayLimit;

        uiManager->update();
    }

    if (((Util::Input::IsKeyPressed(Util::Keycode::RIGHT) && delayCounter <= delayCheck) ||
        (Util::Input::IsKeyDown(Util::Keycode::RIGHT))) && 
        selection->getAbsolutePos().x < mapManager->getMapSize().x - TILE_SIZE){
        
        selection->moveDirectly({ TILE_SIZE, 0 });
        delayCounter = delayLimit;

        uiManager->update();
    }

    if (Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
        LOG_INFO("Enter pressed");
        selection->changeStatus();
        selection->setMoveLimit(characterManager->selectCharacter(selection->getAbsolutePos()));
    }

    //info UI
    if (Util::Input::IsKeyDown(Util::Keycode::F1)) {
        uiManager->changeVisibleTileInfo();
    }
    //tip 
    if (Util::Input::IsKeyDown(Util::Keycode::F2)) {
        characterManager->changeTipsVisible();
    }
    //update
    camera->update();
    if (!--delayCounter) delayCounter = delayLimit;
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}

