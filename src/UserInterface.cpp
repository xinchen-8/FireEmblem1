#include "UserInterface.hpp"

UserInterface::UserInterface(const std::vector<std::shared_ptr<Tile>> &tiles){
	m_Visible = false;
	m_ZIndex = 7;

	for (auto& e : tiles) tileTable.push_back(e);
	setString(" ");
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
	SetPivot({-floor(m_Drawable->GetSize().x/2), -floor(m_Drawable->GetSize().y/2)});
	m_Transform.translation = r_pos;

	for (int j = 0; j < UItileNum.y; j++) {
		for (int i = 0; i < UItileNum.x; i++) {
			form[j][i]->setRelativePos(r_pos + getTileRelativePos({ i, j }));
		}
	}
}

void UserInterface::setRelativePos(){
	SetPivot({-floor(m_Drawable->GetSize().x/2), -floor(m_Drawable->GetSize().y/2)});
}

void UserInterface::setUISize(glm::ivec2 windowNums) {
	UItileNum = windowNums;

	form.clear();
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
	setRelativePos();
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
	setUISize({ 4, 2 });

	setRelativePos({
		- floor(PTSD_Config::WINDOW_WIDTH / 2) + 1 * TILE_SIZE,
		- floor(PTSD_Config::WINDOW_HEIGHT / 2) + 1 * TILE_SIZE
	});
}

void TileInfoUI::load(std::shared_ptr<Tile> tile) {
	if(tile) this->tile = tile;
}

void TileInfoUI::update(){
	setString(tile->getName()+"\navoid +"+std::to_string(tile->getAvoid()));
	// setAbsolutePos({ TILE_SIZE, TILE_SIZE });
}

CharacterInfoUI::CharacterInfoUI(std::vector<std::shared_ptr<Tile>>& tiles) :
	UserInterface(tiles) {
	setUISize({ 5, 3 });

	setRelativePos({
		+ floor(PTSD_Config::WINDOW_WIDTH / 2) - 5 * TILE_SIZE,
		- floor(PTSD_Config::WINDOW_HEIGHT / 2) + 1 * TILE_SIZE
	});
}

void CharacterInfoUI::load(std::shared_ptr<Character> character) {
	if (character) this->character = character;
}

void CharacterInfoUI::update() {
    std::string content=
		character->getName() + "\n" +
		character->getClassName() + "\n" +
		"HP " + std::to_string(character->getCurHP()) + "/" + std::to_string(character->getHpLimit()) + "\n"; //first getHP() will be replaced by getCurHP()
    setString(content);
}

UIManager::UIManager(
	std::shared_ptr<Selection> s,
	std::shared_ptr<MapManager> tm,
	std::shared_ptr<PlayerManager> pm
	) : mapManager(tm), selection(s), playerManager(pm) {	

	for (int i = 0; i < 9; i++) {
		tiles.push_back(std::make_shared<Tile>(
			"F" + std::to_string(i+1),TILE_UI "text" + std::to_string(i) + ".png"));
		tiles[i]->SetZIndex(6);
	}

	tileInfo = std::make_shared<TileInfoUI>(tiles);
	characterInfo = std::make_shared<CharacterInfoUI>(tiles);
	load();
}

void UIManager::load() {
	tileInfo->load(mapManager->getPosTile(selection->getAbsolutePos()));

	auto c = selection->getSelectCharacter();
	if(c) characterInfo->load(c);
	else characterInfo->load(playerManager->getPosCharacter(selection->getAbsolutePos()));
}

void UIManager::update() {
	tileInfo->update();
	characterInfo->update();
}

void UIManager::changeVisibleTileInfo() {
	tileInfo->setVisible(!tileInfo->getVisible());
}

void UIManager::changeVisibleCharacterInfo() {
	characterInfo->setVisible(!characterInfo->getVisible());
}

std::vector<std::shared_ptr<Util::GameObject>> UIManager::getChildren() {
	std::vector<std::shared_ptr<Util::GameObject>> children = {};
	std::vector<std::shared_ptr<Util::GameObject>> reg = tileInfo->getChildren();
	std::vector<std::shared_ptr<Util::GameObject>> c = characterInfo->getChildren();

	for (auto &e : reg) children.push_back(std::static_pointer_cast<Util::GameObject>(e));
	children.push_back(tileInfo);
	for (auto &e : c) children.push_back(std::static_pointer_cast<Util::GameObject>(e));
	children.push_back(characterInfo);
	return children;
}