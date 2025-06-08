#ifndef APP_HPP
#define APP_HPP

#include "Camera.hpp"
#include "ProcessController.hpp"
#include "Tile.hpp"
#include "UserInterface/UserInterface.hpp"
#include "Util/Renderer.hpp"
#include "pch.hpp" // IWYU pragma: export

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
    // void ValidTask();
    int currentLevel = 1;
    bool cheating = false;
    State m_CurrentState = State::START;

    bool accessInput = true;
    const int delayKeyLimit = 10;
    const int delayKeyCheck = 4;
    int delayKeyCounter = delayKeyLimit;

    std::shared_ptr<MapManager> mapManager = nullptr;
    std::shared_ptr<PlayerManager> playerManager = nullptr;
    std::shared_ptr<EnemyManager> enemyManager = nullptr;

    std::shared_ptr<Selection> selection = nullptr;
    std::shared_ptr<UIManager> uiManager = nullptr;
    std::shared_ptr<Camera> camera = nullptr;
    std::shared_ptr<ProcessController> pc = nullptr;
};

#endif
