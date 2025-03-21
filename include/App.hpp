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

    float getCurrentTileScale() const { return Tile_Scale; }

private:
    void ValidTask();

private:
    const float Tile_Scale = TILE_SCALE;
    const int Tile_Size = TILE_SIZE;

    State m_CurrentState = State::START;
    
    std::shared_ptr<TileManager> tileManager = std::make_shared<TileManager>(1);
    std::shared_ptr<Selection> selection = std::make_shared<Selection>();
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(tileManager, selection);;

};

#endif
