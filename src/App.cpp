#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

App::App() : m_Map(1){

}

void App::Start() {
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;
    m_Root.AddChildren(m_Map.getChildren());
    m_Map.startAnimations();
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
    m_Root.Update();
    // m_Map.checkTileFrameWork();
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
