#ifndef UI_HPP
#define UI_HPP

#include "Util/Text.hpp"
#include "config.hpp"
#include "Tile.hpp"
#include "Selection.hpp"
#include "CharacterManager.hpp"
#include "Integration.hpp"

class UserInterface : public Util::GameObject {
public:
	UserInterface(const std::vector<std::shared_ptr<Tile>> &tiles);

	virtual void update() = 0;
	void setVisible(bool visible);
	void setRelativePos(glm::ivec2 r_pos);
	void setRelativePos();
	void setUISize(glm::ivec2 windowNums);
	void setString(std::string content);

	glm::ivec2 getTileRelativePos(glm::ivec2 pos);
	glm::ivec2 getUISize() { return { UItileNum.x * TILE_SIZE, UItileNum.y * TILE_SIZE }; }
	bool getVisible() const { return m_Visible; }
	virtual std::vector<std::shared_ptr<GameObject>> getChildren();

private:
	glm::ivec2 UItileNum = { 0, 0 };
	std::vector<std::vector<std::shared_ptr<Tile>>> form = {};
	std::vector<std::shared_ptr<Tile>> tileTable = {};
};


class TileInfoUI : public UserInterface {
public:
	TileInfoUI(std::vector<std::shared_ptr<Tile>>& tiles);
	void load(std::shared_ptr<Tile> tile);
	void update() override;
private:
	std::shared_ptr<Tile> tile = nullptr;
};

class CharacterInfoUI : public UserInterface {
public:
	CharacterInfoUI(std::vector<std::shared_ptr<Tile>>& tiles);
	void load(std::shared_ptr<Character> character);
	void update() override;
private:
	std::shared_ptr<Character> character = nullptr;
};

class UIManager {
public:
	UIManager(
		std::shared_ptr<Selection> s,
		std::shared_ptr<MapManager> tm,
		std::shared_ptr<PlayerManager> pm
	);
	void load();
	void update();
	void changeVisibleTileInfo();
	void changeVisibleCharacterInfo();
	
	void changeCharacterInfoUIMode();

	std::vector<std::shared_ptr<Util::GameObject>> getChildren();
	std::vector<std::shared_ptr<Util::GameObject>> getCharacterInfoUIChildren();

private:
	std::shared_ptr<MapManager> mapManager = nullptr;
	std::shared_ptr<Selection> selection = nullptr;
	std::shared_ptr<PlayerManager> playerManager = nullptr;

	std::vector<std::shared_ptr<Tile>> tiles = {};
	std::shared_ptr<TileInfoUI> tileInfo = nullptr;
	std::shared_ptr<CharacterInfoUI> characterInfo = nullptr;
	
};

#endif