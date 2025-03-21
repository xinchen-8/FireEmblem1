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

    if (Util::Input::IsKeyPressed(Util::Keycode::UP)) {
        selection->moveDirectly({ 0, TILE_SIZE * TILE_SCALE });
    }
    
    if (Util::Input::IsKeyPressed(Util::Keycode::DOWN)){
        selection->moveDirectly({ 0, -TILE_SIZE * TILE_SCALE });
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)){
        selection->moveDirectly({ -TILE_SIZE * TILE_SCALE, 0 });
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        selection->moveDirectly({ TILE_SIZE * TILE_SCALE, 0 });
    }


    camera->update();
    // m_Map.checkTileFrameWork();
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}

