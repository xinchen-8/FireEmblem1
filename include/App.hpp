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

    const int delayKeyLimit = 10;
    const int delayKeyCheck = 4;
    int delayKeyCounter = delayKeyLimit;
    
    std::shared_ptr<MapManager> mapManager = std::make_shared<MapManager>(1);
    std::shared_ptr<PlayerManager> playerManager = std::make_shared<PlayerManager>(mapManager);
    std::shared_ptr<EnemyManager> enemyManager = std::make_shared<EnemyManager>(mapManager);

    std::shared_ptr<Selection> selection = std::make_shared<Selection>();
    std::shared_ptr<UIManager> uiManager = std::make_shared<UIManager>(
        selection, mapManager
    );
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(
        playerManager, enemyManager, mapManager, uiManager, selection
    );

};

#endif
