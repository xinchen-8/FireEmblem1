#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Tile.hpp"
#include "Util/Renderer.hpp"
#include "Camera.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    App();
    void Start();
    void Update();
    void End(); // NOLINT(readability-convert-member-functions-to-static)


private:
    //void ValidTask();

private:

    State m_CurrentState = State::START;
    const int delayLimit = 10;
    const int delayCheck = 2;
    int delayCounter = delayLimit;
    
    std::shared_ptr<Selection> selection = std::make_shared<Selection>();
    std::shared_ptr<TileManager> tileManager = std::make_shared<TileManager>(1);
    std::shared_ptr<UIManager> uiManager = std::make_shared<UIManager>(
        selection, tileManager
    );
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(
        tileManager, uiManager, selection
    );

};

#endif
