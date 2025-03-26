#include "UserInterface.hpp"

UserInterface::UserInterface(const std::vector<std::shared_ptr<Tile>> &tiles){
	m_Visible = false;
	m_ZIndex = 6;
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

void UserInterface::setRelativePos(glm::vec2 r_pos) {
	m_Transform.translation = r_pos + glm::vec2{
		floor(UItileNum.x * TILE_SIZE / 2.5),
		floor(UItileNum.y * TILE_SIZE / 3.5),
	};

	for (int j = 0; j < UItileNum.y; j++) {
		for (int i = 0; i < UItileNum.x; i++) {
			form[j][i]->setRelativePos(r_pos + getTileRelativePos({ i, j }));
		}
	}
}

void UserInterface::setUISize(glm::vec2 windowNums) {
	UItileNum = windowNums;

	form = {};
	for (int j = 0; j < windowNums.y; j++) {
		std::vector<std::shared_ptr<Tile>> reg = {};

		for (int i = 0; i < windowNums.x; i++) {
			glm::vec2 currentPos = m_Transform.translation + getTileRelativePos({ i, j });

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
		ASSETS FONTPATH, FONT_SIZE, content, Util::Color(255, 255, 255), false
	);
}

glm::vec2 UserInterface::getTileRelativePos(glm::vec2 pos) {
	return glm::vec2{ pos.x * TILE_SIZE, (UItileNum.y - 1 - pos.y) * TILE_SIZE };
}

std::vector<std::shared_ptr<CameraGameObject>> UserInterface::getChildren() {
	std::vector<std::shared_ptr<CameraGameObject>> children;
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
	setAbsolutePos({ TILE_SIZE, TILE_SIZE });
}

UIManager::UIManager(
	std::shared_ptr<Selection> s,
	std::shared_ptr<TileManager> tm
	//characterManager,
	) : tileManager(tm), selection(s) {

	for (int i = 0; i < 9; i++) {
		tiles.push_back(std::make_shared<Tile>(
			"F" + std::to_string(i+1), ASSETS UI_FOLDER "text" + std::to_string(i) + ".png"));
		tiles[i]->SetZIndex(5);
	}

	tileInfo = std::make_shared<TileInfoUI>(tiles);
}

void UIManager::update() {
	auto reg = tileManager->getPosTile(selection->getAbsolutePos());
	if(reg) tileInfo->update(*reg);
}

void UIManager::changeVisibleTileInfo() {
	tileInfo->setVisible(!tileInfo->getVisible());
}

std::vector<std::shared_ptr<CameraGameObject>> UIManager::getChildren() {
	std::vector<std::shared_ptr<CameraGameObject>> children = {};
	std::vector<std::shared_ptr<CameraGameObject>> reg = tileInfo->getChildren();
	children.insert(children.end(), reg.begin(), reg.end());
	children.push_back(tileInfo);
	return children;
}