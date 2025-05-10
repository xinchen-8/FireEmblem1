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

	virtual void update() { return; }
	virtual void setVisible(bool visible);
	void setRelativePos(glm::ivec2 r_pos);
	void setRelativePos();
	void setUISize(glm::ivec2 windowNums);
	void setString(std::string content);

	glm::ivec2 getTileRelativePos(glm::ivec2 pos);
	glm::ivec2 getRelativePos() { return m_Transform.translation; }
	glm::ivec2 getUISize() { return { UItileNum.x * TILE_SIZE, UItileNum.y * TILE_SIZE }; }
	bool getVisible() const { return m_Visible; }
	virtual std::vector<std::shared_ptr<GameObject>> getChildren();

protected:
	glm::ivec2 UItileNum = { 0, 0 };
	std::vector<std::vector<std::shared_ptr<Tile>>> form = {};
	std::vector<std::shared_ptr<Tile>> tileTable = {};
};

class SelectedUI : public UserInterface{ //base on ActUI
public:
	SelectedUI(std::vector<std::shared_ptr<Tile>>& tiles);
	void setVisible(bool visible) override;
	virtual void load(std::vector<bool> flags);
	virtual void update(int listMov);

	std::string getActive(){ return options[selectPoint]; }
	int getSelectPointIndex() { return selectPoint; }
	std::vector<std::shared_ptr<GameObject>> getChildren() override;
protected:
	std::vector<std::string> options;
	std::vector<bool> option_flags;
	int selectPoint = 0;

	std::shared_ptr<Tile> point = std::make_shared<Tile>("Point", TILE_SELECTION "point.png");
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

class ActUI : public SelectedUI {
public:
	ActUI(std::vector<std::shared_ptr<Tile>>& tiles);
	std::string getActive(){ return options[selectPoint]; }
};

class WeaponUI : public SelectedUI {
public:
	WeaponUI(std::vector<std::shared_ptr<Tile>>& tiles);
	void loadWeapon(std::vector<std::shared_ptr<Weapon>> weapons);
};

class UIManager {
public:
	UIManager(
		std::shared_ptr<Selection> s,
		std::shared_ptr<MapManager> tm,
		std::shared_ptr<PlayerManager> pm,
		std::shared_ptr<EnemyManager> em
	);
	void load();
	void update();

	void loadActUI();
	void updateActUI(int listMov) { selectedAct->update(listMov); }
	void activeActUI();

	void loadWeaponUI(glm::ivec2 targetPos);
	void updateWeaponUI(int listMov) { selectedWeapon->update(listMov);}
	void actWeaponUI();

	void changeVisibleTileInfo();
	void changeVisibleCharacterInfo();
	
	std::vector<std::shared_ptr<Util::GameObject>> getChildren();

private:
	std::shared_ptr<MapManager> mapManager = nullptr;
	std::shared_ptr<Selection> selection = nullptr;
	std::shared_ptr<PlayerManager> playerManager = nullptr;
	std::shared_ptr<EnemyManager> enemyManager = nullptr;

	std::vector<std::shared_ptr<Tile>> tiles = {};
	std::shared_ptr<TileInfoUI> tileInfo = nullptr;
	std::shared_ptr<CharacterInfoUI> characterInfo = nullptr;
	std::shared_ptr<ActUI> selectedAct = nullptr;
	std::shared_ptr<WeaponUI> selectedWeapon = nullptr;
};

#endif