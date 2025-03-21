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
    
    //TODO: do your things here and delete this line <3
    
    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    if (((Util::Input::IsKeyPressed(Util::Keycode::UP) && delayCounter <= delayCheck) ||
        (Util::Input::IsKeyDown(Util::Keycode::UP))) &&
        selection->getAbsolutePos().y < tileManager->getMapSize().y - TILE_SCALE*TILE_SIZE) {

        selection->moveDirectly({ 0, TILE_SIZE * TILE_SCALE });
        delayCounter = delayLimit;
    }
    
    if (((Util::Input::IsKeyPressed(Util::Keycode::DOWN) && delayCounter <= delayCheck) ||
        (Util::Input::IsKeyDown(Util::Keycode::DOWN))) &&
        selection->getAbsolutePos().y > 0){

        selection->moveDirectly({ 0, -TILE_SIZE * TILE_SCALE });
        delayCounter = delayLimit;
    }

    if (((Util::Input::IsKeyPressed(Util::Keycode::LEFT) && delayCounter <= delayCheck) ||
        (Util::Input::IsKeyDown(Util::Keycode::LEFT))) &&
        selection->getAbsolutePos().x > 0){

        selection->moveDirectly({ -TILE_SIZE * TILE_SCALE, 0 });
        delayCounter = delayLimit;
    }

    if (((Util::Input::IsKeyPressed(Util::Keycode::RIGHT) && delayCounter <= delayCheck) ||
        (Util::Input::IsKeyDown(Util::Keycode::RIGHT))) && 
        selection->getAbsolutePos().x < tileManager->getMapSize().x - TILE_SCALE * TILE_SIZE){
        
        selection->moveDirectly({ TILE_SIZE * TILE_SCALE, 0 });
        delayCounter = delayLimit;
    }


    camera->update();
    if (!--delayCounter) delayCounter = delayLimit;
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}

