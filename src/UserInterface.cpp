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

SelectedUI::SelectedUI(std::vector<std::shared_ptr<Tile>>& tiles): UserInterface(tiles){
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

void SelectedUI::load(std::vector<bool> flags, bool targetIsPlayer){
	if(targetIsPlayer) options[0] = "Target";
	else options[0] = "Attack";

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
	if(!getVisible()) return;
	for(int i=selectPoint+listMov; i<option_flags.size() && i>=0; i+=listMov){
		if(static_cast<bool>(option_flags[i])){
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

ActUI::ActUI(std::vector<std::shared_ptr<Tile>>& tiles) : SelectedUI(tiles){
	setUISize({ 4, 3 });
	setRelativePos({
		+ floor(PTSD_Config::WINDOW_WIDTH / 2) - 5 * TILE_SIZE,
		+ floor(PTSD_Config::WINDOW_HEIGHT / 2) - 4 * TILE_SIZE
	});

	options = {"Attack","Item", "Wait"};//"Trade"
	option_flags = {0,1,1};
}

WeaponUI::WeaponUI(std::vector<std::shared_ptr<Tile>>& tiles): SelectedUI(tiles){
	setUISize({ 6, 3 });
	setRelativePos({
		+ floor(PTSD_Config::WINDOW_WIDTH / 2) - 7 * TILE_SIZE,
		+ floor(PTSD_Config::WINDOW_HEIGHT / 2) - 4 * TILE_SIZE
	});
}

void WeaponUI::loadWeapon(std::vector<std::shared_ptr<HandHeldItem>> HHitems){
	// std::cout<<std::to_string(weapons.size())<<std::endl;
	options.clear();
	option_flags.clear();

	for(auto &i: HHitems){
		if(i!=nullptr){
			// std::ostringstream reg;
		    // reg << std::left << std::setw(15) << w->getName() 
				// << std::right << std::setw(3) << w->getUses();
			options.push_back(i->getName()+" "+std::to_string(i->getUses()));
			option_flags.push_back(true);
		}
		else{
			options.push_back("null");
			option_flags.push_back(false);
		}
	}

	// for(auto &t: options) std::cout<<t<<std::endl;
	// for(const auto &t: option_flags) std::cout<<std::to_string(t)<<std::endl;
	
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
	while(--spaceCounter) str+="\n";
	setString(str);
	point->setRelativePos(m_Transform.translation+glm::vec2(-TILE_SIZE/2, 1.5 * TILE_SIZE));
}

ItemUI::ItemUI(std::vector<std::shared_ptr<Tile>>& tiles): SelectedUI(tiles){
	setUISize({ 6, 3 });
	m_Transform.scale = {1, 0.8};
	setRelativePos({
		+ floor(PTSD_Config::WINDOW_WIDTH / 2) - 7 * TILE_SIZE,
		+ floor(PTSD_Config::WINDOW_HEIGHT / 2) - 4 * TILE_SIZE
	});
}

void ItemUI::loadItem(std::vector<std::shared_ptr<Item>> items, int handheldIndex){
	options.clear();
	option_flags.clear();

	for(auto &w: items){
		if(w!=nullptr){
			options.push_back(w->getName()+" "+std::to_string(w->getUses()));
			option_flags.push_back(true);
		}
		else{
			options.push_back("null");
			option_flags.push_back(false);
		}
	}

	std::string str = "";
	for(int i=0; i<option_flags.size(); i++) str += options[i] + "\n";
	setString(str);
	if(items[handheldIndex]) selectPoint = handheldIndex;
	point->setRelativePos(m_Transform.translation+glm::vec2(-TILE_SIZE/2, 1.65 * TILE_SIZE - handheldIndex * 30));	
}

void ItemUI::update(int listMov){
	if(!getVisible()) return;
	for(int i=selectPoint+listMov; i<option_flags.size() && i>=0; i+=listMov){
		if(static_cast<bool>(option_flags[i])){
			selectPoint = i;
			glm::vec2 regPos = point->getRelativePos();
			point->setRelativePos(regPos - glm::vec2(0, 30 * listMov));
			return;
		}
	}
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
	selectedAct = std::make_shared<ActUI>(tiles);
	selectedWeapon = std::make_shared<WeaponUI>(tiles);
	selectedItem = std::make_shared<ItemUI>(tiles);
	load();
	tileInfo->setVisible(true);
	characterInfo->setVisible(true);
}

void UIManager::load() {
	tileInfo->load(mapManager->getPosTile(selection->getAbsolutePos()));

	auto s = selection->getSelectCharacter();
	auto c = playerManager->getPosLevelCharacter(selection->getAbsolutePos());
	auto e = enemyManager->getPosLevelCharacter(selection->getAbsolutePos());
	if(c) characterInfo->load(c);
	else if(e) characterInfo->load(e);
	else if(s && selection->getStatus()==SelectionStatus::Moving) characterInfo->load(s);
}

void UIManager::update() {
	tileInfo->update();
	characterInfo->update();
}

void UIManager::loadActUI(){
	// if(selectedAct->getVisible()) return;

	std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();
	std::vector<bool> flags = {};
	flags.push_back(selectedCharacter->getAttackRange().size()!=0); //"Attack"
	flags.push_back(true); //"Item"
	flags.push_back(true); //"Wait"
	//"Trade" not yet
		
	selectedAct->load(flags, (selectedCharacter->getClassName()=="Curate"));
	selectedAct->setVisible(true);
}

void UIManager::activeActUI(){
	std::string act = selectedAct->getActive();
	std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();

	if(act=="Attack" && selectedCharacter){
		LOG_INFO("Select Attack Option");
		selection->setStatus(SelectionStatus::AttackTargeting);

        playerManager->buildCharacterTips(selectedCharacter);
        if(selectedCharacter->getAttackRange().size()==0) LOG_ERROR("NO ENEMY...");            
        selection->setMoveLimit(selectedCharacter->getAttackRange());
        selection->setAbsolutePos(selection->getLimitRange().begin()->first);
	}
	else if(act=="Target" && selectedCharacter){
		LOG_INFO("Select Target Option");
		selection->setStatus(SelectionStatus::Targeting);

        playerManager->buildCharacterTips(selectedCharacter);
        if(selectedCharacter->getAttackRange().size()==0) LOG_ERROR("NO PLAYER...");
        selection->setMoveLimit(selectedCharacter->getAttackRange());
        selection->setAbsolutePos(selection->getLimitRange().begin()->first);
	}
	else if(act=="Item" && selectedCharacter){
		LOG_INFO("Select Item Option");
		selection->setStatus(SelectionStatus::ITEMIUI);
		loadItemUI();
		selectedItem->setVisible(true);
	}
	else if(act=="Wait" && selectedCharacter){
		LOG_INFO("Select Wait Option");
		selection->setStatus(SelectionStatus::Normal);
		selectedCharacter->setStatus(CharacterStatus::Waiting);
		playerManager->removeUnwaitingCharacter(selectedCharacter);
		playerManager->clearTips();
	}
	else LOG_ERROR("NO Act With: " + act);
	selectedAct->setVisible(false);
}

void UIManager::loadWeaponUI(glm::ivec2 targetPos, bool isHealSpecialCase){
	// if(selectedWeapon->getVisible()) return;

	std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();
	int steps = 0;
	glm::ivec2 distance = selectedCharacter->getAbsolutePos() - targetPos;
	steps += abs(distance.x)/TILE_SIZE;
	steps += abs(distance.y)/TILE_SIZE;

	std::vector<std::shared_ptr<Item>> items = selectedCharacter->getItems();
	std::vector<std::shared_ptr<HandHeldItem>> HHitems = {nullptr, nullptr, nullptr, nullptr};
	
	if(isHealSpecialCase){
		for (int i=0; i<items.size(); i++){
			auto heal = std::dynamic_pointer_cast<Heal>(items[i]);
			if(!heal) HHitems[i] = nullptr;
			else HHitems[i] = std::dynamic_pointer_cast<HandHeldItem>(items[i]);
		}
	}
	else{
		for (int i=0; i<items.size(); i++){
			auto weapon = std::dynamic_pointer_cast<Weapon>(items[i]);
			if(!weapon) HHitems[i] = nullptr;
			else{
				for(auto &r: weapon->getRng()){
					if(r==steps){
						HHitems[i] = std::dynamic_pointer_cast<HandHeldItem>(items[i]);
						break;
					}
				}
			}
		}
	}
	selectedWeapon->loadWeapon(HHitems);
	selectedWeapon->setVisible(true);
}

void UIManager::actWeaponUI(){
	std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();
	selectedCharacter->setHandHeldItemWithIndex(selectedWeapon->getSelectPointIndex());
	selectedWeapon->setVisible(false);
}

void UIManager::loadItemUI(){
	std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();
	std::vector<std::shared_ptr<Item>> items = selectedCharacter->getItems();
	int index = selectedCharacter->getHandHeldIndex();

	selectedItem->loadItem(items, index);
	selectedItem->setVisible(true);
}

void UIManager::actItemUI(){
	std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();
	std::shared_ptr<Item> item = selectedCharacter->getItems()[selectedItem->getSelectPointIndex()];
	std::shared_ptr<HandHeldItem> hhi = std::dynamic_pointer_cast<HandHeldItem>(item);
	std::shared_ptr<Vulnerary> vi = std::dynamic_pointer_cast<Vulnerary>(item);

	if(hhi){
		selectedCharacter->setHandHeldItemWithIndex(selectedItem->getSelectPointIndex());
		LOG_INFO("Now " + selectedCharacter->getName() + " Hand Holding the "+hhi->getName()+". ");
	}
	else if(vi){
		selectedCharacter->useVulnerary(selectedItem->getSelectPointIndex());
	}
	else LOG_ERROR("unkown Item Error!");
	selectedItem->setVisible(false);
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
	std::vector<std::shared_ptr<Util::GameObject>> a = selectedAct->getChildren();
	std::vector<std::shared_ptr<Util::GameObject>> w = selectedWeapon->getChildren();
	std::vector<std::shared_ptr<Util::GameObject>> i = selectedItem->getChildren();

	for (auto &e : reg) children.push_back(std::static_pointer_cast<Util::GameObject>(e));
	children.push_back(tileInfo);
	for (auto &e : c) children.push_back(std::static_pointer_cast<Util::GameObject>(e));
	children.push_back(characterInfo);
	for (auto &e : a) children.push_back(std::static_pointer_cast<Util::GameObject>(e));
	children.push_back(selectedAct);
	for (auto &e : w) children.push_back(std::static_pointer_cast<Util::GameObject>(e));
	children.push_back(selectedWeapon);
	for (auto &e : i) children.push_back(std::static_pointer_cast<Util::GameObject>(e));
	children.push_back(selectedItem);
	return children;
}