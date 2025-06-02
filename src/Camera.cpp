#include "Camera.hpp"

Camera::Camera() {}

void Camera::set(std::shared_ptr<PlayerManager> pm, std::shared_ptr<EnemyManager> em, std::shared_ptr<MapManager> tm,
                 std::shared_ptr<UIManager> ui, std::shared_ptr<Selection> s) {
    playerManager = pm;
    enemyManager = em;
    mapManager = tm;
    uiManager = ui;
    selection = s;

    // add children
    std::vector<std::shared_ptr<CameraGameObject>> tiles = mapManager->getChildren();
    children.insert(children.end(), tiles.begin(), tiles.end());
    tiles = playerManager->getChildren();
    children.insert(children.end(), tiles.begin(), tiles.end());
    tiles = enemyManager->getChildren();
    children.insert(children.end(), tiles.begin(), tiles.end());
    children.push_back(selection);

    std::vector<std::shared_ptr<Util::GameObject>> ui_tiles = uiManager->getChildren();
    UIchildren.insert(UIchildren.end(), ui_tiles.begin(), ui_tiles.end());

    for (auto &element : children) {
        renderer.AddChild(element);
    }
    for (auto &element : ui_tiles) {
        renderer.AddChild(element);
    }
}

void Camera::resetCameraAbsolutePos() {
    glm::ivec2 mapSize = mapManager->getMapSize();
    absolutePos = selection->getAbsolutePos();

    glm::ivec2 BorderDistance = {round(PTSD_Config::WINDOW_WIDTH / (TILE_SIZE * 2)) * TILE_SIZE,
                                 round(PTSD_Config::WINDOW_HEIGHT / (TILE_SIZE * 2)) * TILE_SIZE};

    if (absolutePos.x < BorderDistance.x)
        absolutePos.x = BorderDistance.x;
    else if (absolutePos.x >= mapSize.x - BorderDistance.x - TILE_SIZE)
        absolutePos.x = mapSize.x - BorderDistance.x - TILE_SIZE;
    if (absolutePos.y < BorderDistance.y)
        absolutePos.y = BorderDistance.y;
    else if (absolutePos.y >= mapSize.y - BorderDistance.y - TILE_SIZE)
        absolutePos.y = mapSize.y - BorderDistance.y - TILE_SIZE;
}

void Camera::setChildrenRelativePos() {
    for (auto &element : children) {
        element->m_Transform.translation = element->getAbsolutePos() - absolutePos;
    }
}

void Camera::update() {
    glm::ivec2 BorderDistance = {round(PTSD_Config::WINDOW_WIDTH / (TILE_SIZE * 2)) * TILE_SIZE,
                                 round(PTSD_Config::WINDOW_HEIGHT / (TILE_SIZE * 2)) * TILE_SIZE};
    glm::ivec2 mapSize = mapManager->getMapSize();
    glm::ivec2 selectionPos = selection->getAbsolutePos();

    if (selectionPos.x >= BorderDistance.x && selectionPos.x <= mapSize.x - BorderDistance.x - TILE_SIZE)
        absolutePos.x = selectionPos.x;
    if (selectionPos.y >= BorderDistance.y && selectionPos.y <= mapSize.y - BorderDistance.y - TILE_SIZE)
        absolutePos.y = selectionPos.y;

    setChildrenRelativePos();
    renderer.Update();
}

void Camera::addChildren(std::vector<std::shared_ptr<Util::GameObject>> children) {
    renderer.AddChildren(children);
    UIchildren.insert(UIchildren.end(), children.begin(), children.end());
}
