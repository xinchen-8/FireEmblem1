#ifndef UI_HPP
#define UI_HPP

#include "Util/Text.hpp"
#include "config.hpp"
#include "Tile.hpp"
#include "Selection.hpp"

class UserInterface : public Util::GameObject {
public:
	UserInterface(const std::vector<std::shared_ptr<Tile>> &tiles);

	void virtual update() = 0;
	void setVisible(bool visible);
	void setRelativePos(glm::vec2 r_pos);
	void setUISize(glm::vec2 windowNums);
	void setString(std::string content);

	glm::vec2 getTileRelativePos(glm::vec2 pos);
	glm::vec2 getUISize() { return { UItileNum.x * TILE_SIZE, UItileNum.y * TILE_SIZE }; }
	bool getVisible() const { return m_Visible; }
	virtual std::vector<std::shared_ptr<GameObject>> getChildren();

private:
	glm::vec2 UItileNum = { 0, 0 };
	std::vector<std::vector<std::shared_ptr<Tile>>> form = {};
	std::vector<std::shared_ptr<Tile>> tileTable = {};
};


class TileInfoUI : public UserInterface {
public:
	TileInfoUI(std::vector<std::shared_ptr<Tile>>& tiles);
	void update(Tile tile);
	void update(){}
};


class UIManager {
public:
	UIManager(
		std::shared_ptr<Selection> s,
		std::shared_ptr<MapManager> tm
		//characterManager
	);
	void update();
	void changeVisibleTileInfo();
	std::vector<std::shared_ptr<Util::GameObject>> getChildren();

private:
	std::shared_ptr<MapManager> mapManager;
	std::shared_ptr<Selection> selection;
		//characterManager,
	std::vector<std::shared_ptr<Tile>> tiles = {};
	std::shared_ptr<TileInfoUI> tileInfo = nullptr;
	
};

#endif