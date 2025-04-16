#include "UserInterface.hpp"

UserInterface::UserInterface(const std::vector<std::shared_ptr<Tile>> &tiles){
	m_Visible = false;
	m_ZIndex = 7;
	for (auto& e : tiles) tileTable.push_back(e);
}

void UserInterface::setVisible(bool visible) {
	m_Visible = visible;
	for (auto& row : form) {
		for (auto& e : row) {
			e->SetVisible(visible);
		}
	}
}

void UserInterface::setRelativePos(glm::ivec2 r_pos) {
	m_Transform.translation = r_pos + glm::ivec2{
		floor(UItileNum.x * TILE_SIZE / 2.7),
		floor(UItileNum.y * TILE_SIZE / 3.5),
	};

	for (int j = 0; j < UItileNum.y; j++) {
		for (int i = 0; i < UItileNum.x; i++) {
			form[j][i]->setRelativePos(r_pos + getTileRelativePos({ i, j }));
		}
	}
}

void UserInterface::setUISize(glm::ivec2 windowNums) {
	UItileNum = windowNums;

	form = {};
	for (int j = 0; j < windowNums.y; j++) {
		std::vector<std::shared_ptr<Tile>> reg = {};

		for (int i = 0; i < windowNums.x; i++) {
			glm::ivec2 currentPos = glm::ivec2(m_Transform.translation) + getTileRelativePos({ i, j });

			if (i == 0 && j == 0)
				reg.push_back(std::make_shared<Tile>(*tileTable[0], currentPos));
			else if (i == windowNums.x - 1 && j == 0)
				reg.push_back(std::make_shared<Tile>(*tileTable[2], currentPos));
			else if (i == 0 && j == windowNums.y - 1)
				reg.push_back(std::make_shared<Tile>(*tileTable[6], currentPos));
			else if (i == windowNums.x - 1 && j == windowNums.y - 1)
				reg.push_back(std::make_shared<Tile>(*tileTable[8], currentPos));
			else if (i == windowNums.x - 1)
				reg.push_back(std::make_shared<Tile>(*tileTable[5], currentPos));
			else if (j == windowNums.y - 1)
				reg.push_back(std::make_shared<Tile>(*tileTable[7], currentPos));
			else if (j == 0)
				reg.push_back(std::make_shared<Tile>(*tileTable[1], currentPos));
			else if (i == 0)
				reg.push_back(std::make_shared<Tile>(*tileTable[3], currentPos));
			else
				reg.push_back(std::make_shared<Tile>(*tileTable[4], currentPos));
			
			reg.back()->SetVisible(false);
		}
		form.push_back(reg);
	}
}

void UserInterface::setString(std::string content) {
	m_Drawable = std::make_shared<Util::Text>(
		FONTPATH, FONT_SIZE, content, Util::Color(255, 255, 255), false
	);
}

glm::ivec2 UserInterface::getTileRelativePos(glm::ivec2 pos) {
	return glm::ivec2{ pos.x * TILE_SIZE, (UItileNum.y - 1 - pos.y) * TILE_SIZE };
}

std::vector<std::shared_ptr<Util::GameObject>> UserInterface::getChildren() {
	std::vector<std::shared_ptr<Util::GameObject>> children;
	for (auto& row : form) {
		for (auto& tile : row) {
			children.push_back(tile);
		}
	}
	return children;
}

TileInfoUI::TileInfoUI(std::vector<std::shared_ptr<Tile>>& tiles) :
	UserInterface(tiles){
	setUISize({ 3,2 });
	setRelativePos({
		 - floor(PTSD_Config::WINDOW_WIDTH / 2) + 1 * TILE_SIZE, 
		 - floor(PTSD_Config::WINDOW_HEIGHT / 2) + 1 * TILE_SIZE
	});
}

void TileInfoUI::update(Tile tile) {
	setString(tile.getName()+"\navoid +"+std::to_string(tile.getAvoid()));
	// setAbsolutePos({ TILE_SIZE, TILE_SIZE });
}

UIManager::UIManager(
	std::shared_ptr<Selection> s,
	std::shared_ptr<MapManager> tm
	//characterManager,
	) : mapManager(tm), selection(s) {

	for (int i = 0; i < 9; i++) {
		tiles.push_back(std::make_shared<Tile>(
			"F" + std::to_string(i+1),TILE_UI "text" + std::to_string(i) + ".png"));
		tiles[i]->SetZIndex(6);
	}

	tileInfo = std::make_shared<TileInfoUI>(tiles);
}

void UIManager::update() {
	auto reg = mapManager->getPosTile(selection->getAbsolutePos());
	if(reg) tileInfo->update(*reg);
}

void UIManager::changeVisibleTileInfo() {
	tileInfo->setVisible(!tileInfo->getVisible());
}

std::vector<std::shared_ptr<Util::GameObject>> UIManager::getChildren() {
	std::vector<std::shared_ptr<Util::GameObject>> children = {};
	std::vector<std::shared_ptr<Util::GameObject>> reg = tileInfo->getChildren();
	for (auto &e : reg) children.push_back(std::static_pointer_cast<Util::GameObject>(e));
	children.push_back(tileInfo);
	return children;
}