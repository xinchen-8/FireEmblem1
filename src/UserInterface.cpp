#include "UserInterface.hpp"
#include "Integration.hpp"

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

ImageUI::ImageUI(){}
ImageUI::ImageUI(const std::string& filepath) {
	image = std::make_shared<Util::Image>(filepath);
}

void ImageUI::setImage(const std::string& filepath) {
	if (!image) image = std::make_shared<Util::Image>(filepath);
	else image->SetImage(filepath);
	m_Drawable = image;
}

void ImageUI::setRelativePos(glm::ivec2 pos) {
	m_Transform.translation = pos;
}

void ImageUI::setVisible(bool visible) {
	m_Visible = visible;
}

void ImageUI::setZIndex(int z) {
	m_ZIndex = z;
}

void ImageUI::Draw(const Core::Matrices& data) {
	if (m_Visible && image) image->Draw(data);
}

std::vector<std::shared_ptr<Util::GameObject>> ImageUI::getChildren() {
	return {};
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

CharacterInfoUIFull::CharacterInfoUIFull(std::vector<std::shared_ptr<Tile>>& tiles) :
	UserInterface(tiles) {
	setUISize({ 5, 8 });

	setRelativePos({
		- floor(PTSD_Config::WINDOW_WIDTH / 2) + 1 * TILE_SIZE,
		- floor(PTSD_Config::WINDOW_HEIGHT / 2) + 3 * TILE_SIZE
	});
	profile = std::make_shared<ImageUI>();
	profile->setZIndex(m_ZIndex + 1);
	profile->setRelativePos({
		- floor(PTSD_Config::WINDOW_WIDTH / 2) + 1 * TILE_SIZE,
		- floor(PTSD_Config::WINDOW_HEIGHT / 2) + 3 * TILE_SIZE
	});
}

void CharacterInfoUIFull::load(std::shared_ptr<Character> character) {
	if (character) this->character = character;
}

void CharacterInfoUIFull::update() {
	std::string content=
		character->getName() +"\n" +
		character->getClassName() + "\n" +
		"Lv  " + std::to_string(character->getLevel()) + "  Exp " + std::to_string(character->getExp()) + "\n" +
		"HP  " + std::to_string(character->getCurHP()) + " / " + std::to_string(character->getHpLimit()) + "\n" +
		"Str " + std::to_string(character->getStr()) + "  Lck " + std::to_string(character->getLck()) + "\n" +
		"Skl " + std::to_string(character->getSkl()) + "  Def " + std::to_string(character->getDef()) + "\n" +
		"Wlv " + std::to_string(character->getWlv()) + "  Res " + std::to_string(character->getRes()) + "\n" +
		"Spd " + std::to_string(character->getSpd()) + "  Mov " + std::to_string(character->getMov());
	setString(content);
	std::string profilePath = TILE_PROFILE + character->getName() + "_0.png";
	profile->setImage(profilePath);
}

void CharacterInfoUIFull::setVisible(bool visible) {
    m_Visible = visible;
	UserInterface::setVisible(visible);
    if (profile) profile->setVisible(visible);
}

std::vector<std::shared_ptr<Util::GameObject>> CharacterInfoUIFull::getChildren() {
    auto children = UserInterface::getChildren();
    if (profile) children.push_back(profile);
    return children;
}

SelectedUI::SelectedUI(std::vector<std::shared_ptr<Tile>>& tiles) : UserInterface(tiles){
	setUISize({ 4, 3 });
	setRelativePos({
		+ floor(PTSD_Config::WINDOW_WIDTH / 2) - 5 * TILE_SIZE,
		+ floor(PTSD_Config::WINDOW_HEIGHT / 2) - 4 * TILE_SIZE
	});
	point->SetZIndex(8);
	setVisible(false);
}

void SelectedUI::setVisible(bool visible){
	m_Visible = visible;
	point->setVisible(visible);
	for (auto& row : form) {
		for (auto& e : row) {
			e->SetVisible(visible);
		}
	}
}

void SelectedUI::load(std::vector<bool> flags){	
	if( flags.size() == option_flags.size()){
		option_flags = flags;
		for(int i=0; i<option_flags.size(); i++){
			if(option_flags[i]){
				selectPoint = i;
				break;
			}
		}
		
		int spaceCounter = 0;
		std::string str = "";
		for(int i=0; i<option_flags.size(); i++){
			if(option_flags[i]) str += options[i] + "\n";
			else spaceCounter++;
		}
		while(spaceCounter--) str+="\n";
		setString(str);
		point->setRelativePos(m_Transform.translation+glm::vec2(-TILE_SIZE/2, 1.5 * TILE_SIZE)); // offset
	}
	else LOG_ERROR("Not Correct Input Flags!");
}

void SelectedUI::update(int listMov){
	for(int i=selectPoint+listMov; i<option_flags.size() && i>=0; i+=listMov){
		if(option_flags[i]){
			selectPoint = i;
			glm::vec2 regPos = point->getRelativePos();
			point->setRelativePos(regPos - glm::vec2(0, 40 * listMov));
			return;
		}
	}
}

std::vector<std::shared_ptr<Util::GameObject>> SelectedUI::getChildren(){
	std::vector<std::shared_ptr<Util::GameObject>> children;
	children.push_back(point);
	for (auto& row : form) {
		for (auto& tile : row) {
			children.push_back(tile);
		}
	}
	return children;
}

UIManager::UIManager(
	std::shared_ptr<Selection> s,
	std::shared_ptr<MapManager> tm,
	std::shared_ptr<PlayerManager> pm,
	std::shared_ptr<EnemyManager> em
	) : mapManager(tm), selection(s), playerManager(pm), enemyManager(em) {	

	for (int i = 0; i < 9; i++) {
		tiles.push_back(std::make_shared<Tile>(
			"F" + std::to_string(i+1),TILE_UI "text" + std::to_string(i) + ".png"));
		tiles[i]->SetZIndex(6);
	}

	tileInfo = std::make_shared<TileInfoUI>(tiles);
	characterInfo = std::make_shared<CharacterInfoUI>(tiles);
	characterInfoFull = std::make_shared<CharacterInfoUIFull>(tiles);
	selected = std::make_shared<SelectedUI>(tiles);
	load();
	tileInfo->setVisible(true);
	characterInfo->setVisible(true);
	characterInfoFull->setVisible(true);
}

void UIManager::load() {
	tileInfo->load(mapManager->getPosTile(selection->getAbsolutePos()));

	auto s = selection->getSelectCharacter();
	auto c = playerManager->getPosLevelCharacter(selection->getAbsolutePos());
	auto e = enemyManager->getPosLevelCharacter(selection->getAbsolutePos());
	if(c){
		characterInfo->load(c);
		characterInfoFull->load(c);
	}
	else if(e){
		characterInfo->load(e);
		characterInfoFull->load(e);
	}
	else if(s && selection->getStatus()==SelectionStatus::Moving){
		characterInfo->load(s);
		characterInfoFull->load(s);
	}
}

void UIManager::update() {
	tileInfo->update();
	characterInfo->update();
	characterInfoFull->update();
}

void UIManager::loadSelectedUI(){
	if(selected->getVisible()) return;

	std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();
	std::vector<bool> flags = {};
	flags.push_back(selectedCharacter->getAttackRange().size()!=0); //"Attack"
	flags.push_back(true); //"Item"
	flags.push_back(true); //"Wait"
	//"Trade" not yet
		
	selected->load(flags);
	selected->setVisible(true);
}

void UIManager::activeSelectedUI(){
	std::string act = selected->getActive();
	std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();

	if(act=="Attack" && selectedCharacter){
		LOG_INFO("Select Attack Option");
		selection->setStatus(SelectionStatus::AttackTargeting);

        playerManager->buildCharacterTips(selectedCharacter);
        if(selectedCharacter->getAttackRange().size()==0) LOG_ERROR("NO ENEMY...");            
        selection->setMoveLimit(selectedCharacter->getAttackRange());
        selection->setAbsolutePos(selection->getLimitRange().begin()->first);
	}
	else if(act=="Item"){
		LOG_INFO("Select Item Option");
		LOG_WARN("IUI is not yet!");
		selection->setStatus(SelectionStatus::ITEMIUI);

	}
	else if(act=="Wait" && selectedCharacter){
		LOG_INFO("Select Wait Option");
		selection->setStatus(SelectionStatus::Normal);
		selectedCharacter->setStatus(CharacterStatus::Waiting);
		playerManager->removeUnwaitingCharacter(selectedCharacter);
		playerManager->clearTips();
	}
	else LOG_ERROR("NO Act With: " + act);
	selected->setVisible(false);
}

void UIManager::changeVisibleTileInfo() {
	tileInfo->setVisible(!tileInfo->getVisible());
}

void UIManager::changeVisibleCharacterInfo() {
	characterInfo->setVisible(!characterInfo->getVisible());
}

void UIManager::changeVisibleCharacterInfoFull() {
	characterInfoFull->setVisible(!characterInfoFull->getVisible());
}

std::vector<std::shared_ptr<Util::GameObject>> UIManager::getChildren() {
	std::vector<std::shared_ptr<Util::GameObject>> children = {};
	std::vector<std::shared_ptr<Util::GameObject>> reg = tileInfo->getChildren();
	std::vector<std::shared_ptr<Util::GameObject>> c = characterInfo->getChildren();
	std::vector<std::shared_ptr<Util::GameObject>> cf = characterInfoFull->getChildren();
	std::vector<std::shared_ptr<Util::GameObject>> s = selected->getChildren();

	for (auto &e : reg) children.push_back(std::static_pointer_cast<Util::GameObject>(e));
	children.push_back(tileInfo);
	for (auto &e : c) children.push_back(std::static_pointer_cast<Util::GameObject>(e));
	children.push_back(characterInfo);
	for (auto &e : cf) children.push_back(std::static_pointer_cast<Util::GameObject>(e));
	children.push_back(characterInfoFull);
	for (auto &e : s) children.push_back(std::static_pointer_cast<Util::GameObject>(e));
	children.push_back(selected);
	return children;
}