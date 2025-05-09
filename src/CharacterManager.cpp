#include "CharacterManager.hpp"

CharacterManager::CharacterManager(std::shared_ptr<MapManager> mm):mapManager(mm){

}

// cost time too long => crash
// void CharacterManager::refreshAllCharacterMoveRange(){
// 	std::cout<<"meow1"<<std::endl;

// 	std::unordered_set<glm::ivec2> mask;
// 	if (auto cm = characterManager.lock()) {
// 		for(auto ac: cm->getChildren()) 
// 			mask.insert(ac->getAbsolutePos());
// 	}
// 	std::cout<<"meow2"<<std::endl;

// 	for(auto c: characters){
// 		c->refreshMoveRange(mask);
// 		std::cout<<"meow2meow"<<std::endl;
// 	}
// 	std::cout<<"meow3"<<std::endl;

// }

// void CharacterManager::buildCharacterTips(){
// 	clearTips();

// 	for(auto c: characters){
// 		glm::ivec2 c_pos = c->getAbsolutePos();
// 		if(c_pos.x==-1 && c_pos.y==-1) continue;

// 		for(auto [pos, mov]: c->getMoveRange()){
// 			std::shared_ptr<Tile> tip = getTipTile(pos);
// 			tip->setStart();
// 			std::vector<std::string> r = {TILE_SELECTION "tip0.png"};
// 			tip->setAnimation( std::make_shared<Util::Animation>(
// 				r, true, TILE_INTERVAL, true, 0)
// 			);
// 			tip->SetVisible(true);
// 		}
// 	}
// }

void CharacterManager::removeUnwaitingCharacter(std::shared_ptr<Character> c){
	auto it = std::find(currentUnwaitedCharacters.begin(), currentUnwaitedCharacters.end(), c);
    if (it != currentUnwaitedCharacters.end()) {
        currentUnwaitedCharacters.erase(it);
		LOG_INFO("Remove "+c->getName() +" From UnwaitingCharacterVector.");
    }
}

void CharacterManager::reloadUnwaitingCharacter(){
	for(auto &c: currentLevelCharacters){
		c->setStatus(CharacterStatus::Normal);
		currentUnwaitedCharacters.push_back(c);
	}
}

void CharacterManager::clearTips() {
	for(auto &r: tips){
		for(auto &t: r){
			t->setVisible(false);
		}
	}
}

bool CharacterManager::update(){
	for(auto &c: currentLevelCharacters){
		if(c->getCurHP()<=0){
			auto it = std::find(currentLevelCharacters.begin(), currentLevelCharacters.end(), c);
			if (it != currentLevelCharacters.end()) {
				currentLevelCharacters.erase(it);
				LOG_INFO("Remove "+c->getName() +" From LevelCharacterVector.");
				break;
			}
		}
	}
	for(auto &c: currentUnwaitedCharacters){
		c -> walkDirectly();
	}
	return false;
}

bool CharacterManager::isNoMovableCharacter(){
	return currentUnwaitedCharacters.size()==0;
}

std::shared_ptr<Character> CharacterManager::getPosMovableCharacter(glm::ivec2 a_pos){
	for(std::shared_ptr<Character> &c: currentUnwaitedCharacters){

		if(c->getAbsolutePos() == a_pos) return c;
	}
	return nullptr;
}

std::shared_ptr<Character> CharacterManager::getPosLevelCharacter(glm::ivec2 a_pos){
	for(std::shared_ptr<Character> &c: currentLevelCharacters){

		if(c->getAbsolutePos() == a_pos) return c;
	}
	return nullptr;
}

std::unordered_set<glm::ivec2> CharacterManager::getCharacterPos(){
	std::unordered_set<glm::ivec2> pos = {};
	for(auto &c: currentLevelCharacters){
		pos.insert(c->getAbsolutePos());
	}
	return pos;
}

std::shared_ptr<Tile> CharacterManager::getTipTile(glm::ivec2 a_pos){
	float i = a_pos.x/TILE_SIZE;
	float j = mapManager->getMapSize().y/TILE_SIZE-a_pos.y/TILE_SIZE-1;

	if(i>=0 && i<mapManager->getMapSize().x/TILE_SIZE && j>=0 && j<mapManager->getMapSize().y/TILE_SIZE){
		// std::cout<<"tip: " << i << ", " << j << std::endl;
		return tips[j][i];
	}
	return nullptr;
}

std::vector<std::shared_ptr<CameraGameObject>> CharacterManager::getChildren(){
	std::vector<std::shared_ptr<CameraGameObject>> children = {};
	for(auto &c: characters) children.push_back(c);
	for(auto &r: tips) for(auto &t: r) children.push_back(t);
	return children;
}

PlayerManager::PlayerManager(std::shared_ptr<MapManager> mm):
	CharacterManager(mm) {

	std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile(DATA_CHARACTER "class_cost.csv");
	if(!data) LOG_ERROR("Character class cost loading failed!");

	std::vector<std::string> reg = (*data)[0];
	data->erase(data->begin());

	for(auto &e: *data){
		std::string name = e[COST_INDEX::CLASS];
		std::unordered_map<std::string, int> reg_cost = {};

		for(std::size_t i=COST_INDEX::DEFAULT; i<=COST_INDEX::WALL; i++) reg_cost[reg[i]] = std::stoi(e[i]);
		costTable[name] = std::make_shared<std::unordered_map<std::string, int>>(reg_cost);
	}

	loadCharacter();
	setInitialLevel(mm->getLevel());

	//tips
	std::shared_ptr<Tile> tip = std::make_shared<Tile>("movable", TILE_SELECTION "tip0.png");
	tip->SetZIndex(2);
	int y = mapManager->getMapTileSize().y;
	int x = mapManager->getMapTileSize().x;

	for(int i = 0; i < y; i++){
		std::vector<std::shared_ptr<Tile>> r = {};
		for(int j = 0; j < x; j++){
			std::shared_ptr<Tile> t = std::make_shared<Tile>(*tip, mapManager->getTileAbsolutePos({j, i}));
			t->setVisible(false);
			r.push_back(t);
		}
		tips.push_back(r);
	}
}

void PlayerManager::loadCharacter(){
	//character sequence of all of data must be the same
	//items
	std::shared_ptr<std::vector<std::vector<std::string>>> weaponData = Tool::inputFile( DATA_ITEM "weapon_data.csv");
	std::shared_ptr<std::vector<std::vector<std::string>>> itemData = Tool::inputFile( DATA_ITEM "items_data.csv");
	
	std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile( DATA_CHARACTER "players/player_base_data.csv");
	std::shared_ptr<std::vector<std::vector<std::string>>> g_data = Tool::inputFile( DATA_CHARACTER "players/player_growth_rate.csv");
	std::shared_ptr<std::vector<std::vector<std::string>>> w_data = Tool::inputFile( DATA_CHARACTER "players/player_starting_items.csv");
	
	data->erase(data->begin());
	g_data->erase(g_data->begin());
	w_data->erase(w_data->begin());
	for(std::size_t i = 0; i<data->size(); i++){
		std::vector<std::string> e = (*data)[i];
		std::vector<std::string> g = (*g_data)[i];
		std::vector<std::string> w = (*w_data)[i];

		characters.push_back(
			std::make_shared<Character>(
				mapManager, e, g, costTable[e[CHARACTER_INDEX::CLASS]], true
			)
		);
		characters.back()->setTileAnimation();
		//build items
		for(int j=1; j<4; j++){
			if(w[j]=="0") break;
			// std::cout<<w[j]<<std::endl;
			if (w[j] == "Vulnerary")
			characters.back()->pushItem(std::make_shared<Vulnerary>((*itemData)[ITEM_ROWINDEX::VULNERARY]));
			else if (w[j] == "Heal")
				characters.back()->pushItem(std::make_shared<Heal>((*weaponData)[HANDHELD_ROWINDEX::HEAL]));
			else if (w[j] == "Rapier")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::RAPIER]));
			else if (w[j] == "Iron Sword")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::IRON_SWORD])); 
			else if (w[j] == "Steel Sword")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::STEEL_SWORD]));
			else if (w[j] == "Iron Lance")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::IRON_LANCE]));
			else if (w[j] == "Silver Lance")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::SILVER_LANCE])); 
			else if (w[j] == "Javelin")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::JAVELIN])); 
			else if (w[j] == "Iron Bow")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::IRON_BOW])); 
			else if (w[j] == "Steel Bow")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::STEEL_BOW])); 
			else if (w[j] == "Bowgun")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::BOWGUN])); 
			else if (w[j] == "Iron Axe")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::IRON_AXE]));
			else if (w[j] == "Steel Axe")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::STEEL_AXE]));
			else if (w[j] == "Hand Axe")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::HAND_AXE]));
			else if (w[j] == "Hammer")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::HAMMER]));
		}
	}

	LOG_INFO("Character loading success.");
}

void PlayerManager::setInitialLevel(int level){
	std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile( DATA_CHARACTER "players/player_starting_position.csv");
	
	currentLevelCharacters.clear();
	data->erase(data->begin());

	for(auto &e: *data){
		for(auto &c: characters){
			
			if(c->getName() == e[0]){
				if (e[level*2-1] != "X"){
					currentLevelCharacters.push_back(c);
					c->setAbsolutePos({std::stoi(e[level*2-1]) * TILE_SIZE, std::stoi(e[level*2]) * TILE_SIZE});

					// if (auto cm = characterManager.lock()){
					// 	std::unordered_set<glm::ivec2> mask = cm->getCharacterPos();
						
						// for(auto pos = CHARACTER_UNMOVE[mapManager->getLevel()-1].begin();
						// 	pos != CHARACTER_UNMOVE[mapManager->getLevel()-1].end();
						// 	pos++
						// ) mask.insert(*pos);
						// c->refreshMoveRange(mask);

					// }
					LOG_INFO("Set " + c->getName() + " Position: " + std::to_string(c->getAbsolutePos().x)+ ", " + std::to_string(c->getAbsolutePos().y));
				}
				else{
					c->setAbsolutePos({-1, -1});
					c->setVisible(false);
				}
			}
		}
	}
	currentUnwaitedCharacters = currentLevelCharacters;
}

bool PlayerManager::update(){
	for(auto &c: currentLevelCharacters){
		if(c->getCurHP()<=0){
			auto it = std::find(currentLevelCharacters.begin(), currentLevelCharacters.end(), c);
			if (it != currentLevelCharacters.end()) {
				currentLevelCharacters.erase(it);
				LOG_INFO("Remove "+c->getName() +" From LevelCharacterVector.");
				break;
			}
		}
	}

	for(auto &c: currentLevelCharacters){
		if(c -> walkDirectly()){
			findCharacterAttackTarget(c);
			c->SetZIndex(3);
			return true; //the trigger about character is arrived
		}
	}
	return false;
}

void PlayerManager::changeTipsVisible(std::shared_ptr<Character> character){
	tipsVisible = !tipsVisible;

	if(character){
		buildCharacterTips(character);
	}
}

std::unordered_map<glm::ivec2, int> PlayerManager::selectCharacter(
	std::shared_ptr<Character> character = nullptr){
	if(!character) return {};

	if (auto cm = characterManager.lock()){
		std::unordered_set<glm::ivec2> mask = cm->getCharacterPos();
						
		for(auto pos = CHARACTER_UNMOVE[mapManager->getLevel()-1].begin();
			pos != CHARACTER_UNMOVE[mapManager->getLevel()-1].end();
			pos++
		) mask.insert(*pos);
		character->refreshMoveRange(mask);
	}
	buildCharacterTips(character);
	character->setStatus(CharacterStatus::Moving);
	return character->getMoveRange();
}

void PlayerManager::findCharacterAttackTarget(std::shared_ptr<Character> character){
	if(!character) return;
	character->resetRange();
	character->findAttackScope();
	std::unordered_map<glm::ivec2, int> ar = character->getAttackRange();

	buildCharacterTips(character);
	if (auto cm = characterManager.lock()){
		for(auto &[pos, null]: ar){
			if(!cm->getPosLevelCharacter(pos))
				ar.erase(pos);
		}		
	}
	character->setAttackRange(ar);
}

void PlayerManager::buildCharacterTips(std::shared_ptr<Character> character){
	clearTips();
	//move range
	for(auto [pos, mov]: character->getMoveRange()){
		std::shared_ptr<Tile> tip = getTipTile(pos);
		tip->setStart();
		std::vector<std::string> r = {TILE_SELECTION "tip0.png"};
		tip->setAnimation( std::make_shared<Util::Animation>(
			r, true, TILE_INTERVAL, true, 0)
		);
		tip->SetVisible(tipsVisible);
	}
	//attack range
	for(auto [pos, mov]: character->getAttackRange()){
		std::shared_ptr<Tile> tip = getTipTile(pos);
		tip->setStart();
		std::vector<std::string> r = {TILE_SELECTION "tip1.png"};
		tip->setAnimation( std::make_shared<Util::Animation>(
			r, true, TILE_INTERVAL, true, 0)
		);
		tip->SetVisible(tipsVisible);
	}
}

std::shared_ptr<Character> PlayerManager::getCharacter(std::string id){
	for(std::shared_ptr<Character> &c: characters){
		if(c->getName() == id) return c;
	}
	return nullptr;
}

EnemyManager::EnemyManager(std::shared_ptr<MapManager> mm):
	CharacterManager(mm){

	std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile(DATA_CHARACTER "class_cost.csv");
	if(!data) LOG_ERROR("Enemy class cost loading failed!");

	std::vector<std::string> reg = (*data)[0];
	data->erase(data->begin());
	for(auto &e: *data){
		std::string name = e[COST_INDEX::CLASS];
		std::unordered_map<std::string, int> reg_cost = {};

		for(std::size_t i=COST_INDEX::DEFAULT; i<=COST_INDEX::WALL; i++) reg_cost[reg[i]] = std::stoi(e[i]);
		costTable[name] = std::make_shared<std::unordered_map<std::string, int>>(reg_cost);
	}
	loadCharacter();
	setInitialLevel(mm->getLevel());

	//tips
	std::shared_ptr<Tile> tip = std::make_shared<Tile>("movable", TILE_SELECTION "tip0.png");
	tip->SetZIndex(2);
	int y = mapManager->getMapTileSize().y;
	int x = mapManager->getMapTileSize().x;

	for(int i = 0; i < y; i++){
		std::vector<std::shared_ptr<Tile>> r = {};
		for(int j = 0; j < x; j++){
			std::shared_ptr<Tile> t = std::make_shared<Tile>(*tip, mapManager->getTileAbsolutePos({j, i}));
			t->setVisible(false);
			r.push_back(t);
		}
		tips.push_back(r);
	}
}

void EnemyManager::loadCharacter(){
	std::shared_ptr<std::vector<std::vector<std::string>>> weaponData = Tool::inputFile( DATA_ITEM "weapon_data.csv");
	std::shared_ptr<std::vector<std::vector<std::string>>> itemData = Tool::inputFile( DATA_ITEM "items_data.csv");
	std::shared_ptr<std::vector<std::vector<std::string>>> w_data = Tool::inputFile( DATA_CHARACTER "enemy/enemy_starting_items.csv");

	std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile( DATA_CHARACTER "enemy/enemy_data.csv");
	
	data->erase(data->begin());
	w_data->erase(w_data->begin());
	for(std::size_t i = 0; i<data->size(); i++){
		std::vector<std::string> e = (*data)[i];
		std::vector<std::string> g(9, "0");
		std::vector<std::string> w = (*w_data)[i];
		
		characters.push_back(std::make_shared<Character>(mapManager, e, g, costTable[e[CHARACTER_INDEX::CLASS]], false)); 
		characters.back()->setTileAnimation();
		//build items
		for(int j=2; j<6; j++){
			if(w[j]=="0") break;
			
			if (w[j] == "Vulnerary")
			characters.back()->pushItem(std::make_shared<Vulnerary>((*itemData)[ITEM_ROWINDEX::VULNERARY]));
			else if (w[j] == "Heal")
				characters.back()->pushItem(std::make_shared<Heal>((*weaponData)[HANDHELD_ROWINDEX::HEAL]));
			else if (w[j] == "Rapier")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::RAPIER]));
			else if (w[j] == "Iron Sword")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::IRON_SWORD])); 
			else if (w[j] == "Steel Sword")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::STEEL_SWORD]));
			else if (w[j] == "Iron Lance")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::IRON_LANCE]));
			else if (w[j] == "Silver Lance")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::SILVER_LANCE])); 
			else if (w[j] == "Javelin")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::JAVELIN])); 
			else if (w[j] == "Iron Bow")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::IRON_BOW])); 
			else if (w[j] == "Steel Bow")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::STEEL_BOW])); 
			else if (w[j] == "Bowgun")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::BOWGUN])); 
			else if (w[j] == "Iron Axe")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::IRON_AXE]));
			else if (w[j] == "Steel Axe")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::STEEL_AXE]));
			else if (w[j] == "Hand Axe")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::HAND_AXE]));
			else if (w[j] == "Hammer")
				characters.back()->pushItem(std::make_shared<Weapon>((*weaponData)[HANDHELD_ROWINDEX::HAMMER]));
		}
	}
	
	LOG_INFO("Enemy loading success.");
}

void EnemyManager::setInitialLevel(int level){
	std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile( DATA_CHARACTER "enemy/enemy_starting_position.csv");

	currentLevelCharacters.clear();
	data->erase(data->begin());
	
	for(auto &e: *data){
		for(auto &c: characters){
			if(c->getName() == e[CHARACTER_INDEX::NAME] && c->getClassName() == e[CHARACTER_INDEX::CLASS]){
				std::shared_ptr<Character> currentCharacter = nullptr;

				//clone
				if(c->getAbsolutePos()!=glm::ivec2(0,0)){
					characters.push_back(c->clone());

					if (e[level*2] != "X"){
						currentLevelCharacters.push_back(characters.back());
						characters.back()->setAbsolutePos({std::stoi(e[level*2]) * TILE_SIZE, std::stoi(e[level*2+1]) * TILE_SIZE});
						LOG_INFO("Set Copy of " + characters.back()->getName() + " Position: " + std::to_string(characters.back()->getAbsolutePos().x)+ ", " + std::to_string(characters.back()->getAbsolutePos().y));
					}
					else{
						characters.back()->setAbsolutePos({-1, -1});
						characters.back()->setVisible(false);
					}
				}
				
				//not copy case => set directly
				else if (e[level*2] != "X"){
					currentLevelCharacters.push_back(c);
					c->setAbsolutePos({std::stoi(e[level*2]) * TILE_SIZE, std::stoi(e[level*2+1]) * TILE_SIZE});
					LOG_INFO("Set " + c->getName() + " Position: " + std::to_string(c->getAbsolutePos().x)+ ", " + std::to_string(c->getAbsolutePos().y));
				}
				//not for map1
				else{
					c->setAbsolutePos({-1, -1});
					c->setVisible(false);
				}
				break;
			}
		}
	}
	currentUnwaitedCharacters = currentLevelCharacters;
}
