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

void CharacterManager::clearTips() {
	for(auto &r: tips){
		for(auto &t: r){
			t->setVisible(false);
		}
	}
}

void CharacterManager::update(){
	for(auto &c: characters){
		c -> walkDirectly();
	}
}

std::shared_ptr<Character> CharacterManager::getPosCharacter(glm::ivec2 a_pos){
	for(auto &c: characters){
		if(c->getAbsolutePos() == a_pos) return c;
	}
	return nullptr;
}

std::unordered_set<glm::ivec2>  CharacterManager::getCharacterPos(){
	std::unordered_set<glm::ivec2> pos = {};
	for(auto &c: characters){
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
		std::string name = e[0];
		std::unordered_map<std::string, int> reg_cost = {};

		for(std::size_t i=1; i<e.size(); i++) reg_cost[reg[i]] = std::stoi(e[i]);
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
	std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile( DATA_CHARACTER "players/player_base_data.csv");
	std::shared_ptr<std::vector<std::vector<std::string>>> g_data = Tool::inputFile( DATA_CHARACTER "players/player_growth_rate.csv");
	if(!data) LOG_ERROR("Character loading failed!");
	
	data->erase(data->begin());
	g_data->erase(g_data->begin());
	for(std::size_t i = 0; i<data->size(); i++){
		std::vector<std::string> e = (*data)[i];
		std::vector<std::string> g = (*g_data)[i];

		static std::unordered_map<std::string, std::function<void()>> characterFactory = {
			{"Lord", [&]() { characters.push_back(std::make_shared<Lord>(mapManager, e, g, costTable[e[1]], true)); }},
			{"Cavalier", [&]() { characters.push_back(std::make_shared<Cavalier>(mapManager, e, g, costTable[e[1]], true)); }},
			{"Paladin", [&]() { characters.push_back(std::make_shared<Paladin>(mapManager, e, g, costTable[e[1]], true)); }},
			{"PegasusKnight", [&]() { characters.push_back(std::make_shared<PegasusKnight>(mapManager, e, g, costTable[e[1]], true)); }},
			{"Archer", [&]() { characters.push_back(std::make_shared<Archer>(mapManager, e, g, costTable[e[1]], true)); }},
			{"Knight", [&]() { characters.push_back(std::make_shared<Knight>(mapManager, e, g, costTable[e[1]], true)); }},
			//{"Thief", [&]() { characters.push_back(std::make_shared<Thief>(e, g, costTable[e[1]], true)); }},
			{"Curate", [&]() { characters.push_back(std::make_shared<Curate>(mapManager, e, g, costTable[e[1]], true)); }},
			{"Mercenary", [&]() { characters.push_back(std::make_shared<Mercenary>(mapManager, e, g, costTable[e[1]], true)); }},
			{"Fighter", [&]() { characters.push_back(std::make_shared<Fighter>(mapManager, e, g, costTable[e[1]], true)); }},
			{"Hunter", [&]() { characters.push_back(std::make_shared<Hunter>(mapManager, e, g, costTable[e[1]], true)); }},
			{"Pirate", [&]() { characters.push_back(std::make_shared<Pirate>(mapManager, e, g, costTable[e[1]], true)); }}
		};
	
		
		if (characterFactory.find(e[1]) != characterFactory.end()) {
			characterFactory[e[1]]();
			characters.back()->setTileAnimation();
		} else {
			std::cerr << "Unknown character type: " << e[1] << std::endl;
		}
		
	}
	LOG_INFO("Character loading success.");
}

void PlayerManager::setInitialLevel(int level){
	std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile( DATA_CHARACTER "players/player_starting_position.csv");
	
	data->erase(data->begin());
	for(auto &e: *data){
		for(auto &c: characters){
			
			if(c->getName() == e[0]){
				if (e[level] != "X"){
					c->setAbsolutePos({std::stoi(e[level*2-1]) * TILE_SIZE, std::stoi(e[level*2]) * TILE_SIZE});
					if (auto cm = characterManager.lock()){
						std::unordered_set<glm::ivec2> mask = cm->getCharacterPos();
						
						for(auto pos = CHARACTER_UNMOVE[mapManager->getLevel()-1].begin();
							pos != CHARACTER_UNMOVE[mapManager->getLevel()-1].end();
							pos++
						) mask.insert(*pos);
						c->refreshMoveRange(mask);

					}
					LOG_INFO("Set " + c->getName() + " Position: " + std::to_string(c->getAbsolutePos().x)+ ", " + std::to_string(c->getAbsolutePos().y));
				}
				else{
					c->setAbsolutePos({-1, -1});
					c->setVisible(false);
				}
			}
		}
	}
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
		std::cout<<"meow"<<std::endl;
		for (const auto& pos : cm->getCharacterPos()) {
			std::cout << "(" << pos.x << ", " << pos.y << ")\n";
		}
		
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

void PlayerManager::buildCharacterTips(std::shared_ptr<Character> character){
	clearTips();
	for(auto [pos, mov]: character->getMoveRange()){
		// std::cout<<"meow " << pos.x << ", " << pos.y << std::endl;
		std::shared_ptr<Tile> tip = getTipTile(pos);
		tip->setStart();
		std::vector<std::string> r = {TILE_SELECTION "tip0.png"};
		tip->setAnimation( std::make_shared<Util::Animation>(
			r, true, TILE_INTERVAL, true, 0)
		);
		tip->SetVisible(tipsVisible);
		// std::cout<<"meowwwwww"<<std::endl;
	}
}

std::shared_ptr<Character> PlayerManager::getCharacter(std::string id){
	for(auto &c: characters){
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
		std::string name = e[0];
		std::unordered_map<std::string, int> reg_cost = {};

		for(std::size_t i=1; i<e.size(); i++) reg_cost[reg[i]] = std::stoi(e[i]);
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
	std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile( DATA_CHARACTER "enemy/enemy_data.csv");
	if(!data) LOG_ERROR("Enemy loading failed!");
	
	data->erase(data->begin());
	for(std::size_t i = 0; i<data->size(); i++){
		std::vector<std::string> e = (*data)[i];
		std::vector<std::string> g(9, "0");

		static std::unordered_map<std::string, std::function<void()>> characterFactory = {
			{"Lord", [&]() { characters.push_back(std::make_shared<Lord>(mapManager, e, g, costTable[e[1]], false)); }},
			{"Cavalier", [&]() { characters.push_back(std::make_shared<Cavalier>(mapManager, e, g, costTable[e[1]], false)); }},
			{"Paladin", [&]() { characters.push_back(std::make_shared<Paladin>(mapManager, e, g, costTable[e[1]], false)); }},
			{"PegasusKnight", [&]() { characters.push_back(std::make_shared<PegasusKnight>(mapManager, e, g, costTable[e[1]], false)); }},
			{"Archer", [&]() { characters.push_back(std::make_shared<Archer>(mapManager, e, g, costTable[e[1]], false)); }},
			{"Knight", [&]() { characters.push_back(std::make_shared<Knight>(mapManager, e, g, costTable[e[1]], false)); }},
			{"Thief", [&]() { characters.push_back(std::make_shared<Thief>(mapManager, e, g, costTable[e[1]], false)); }},
			{"Curate", [&]() { characters.push_back(std::make_shared<Curate>(mapManager, e, g, costTable[e[1]], false)); }},
			{"Mercenary", [&]() { characters.push_back(std::make_shared<Mercenary>(mapManager, e, g, costTable[e[1]], false)); }},
			{"Fighter", [&]() { characters.push_back(std::make_shared<Fighter>(mapManager, e, g, costTable[e[1]], false)); }},
			{"Hunter", [&]() { characters.push_back(std::make_shared<Hunter>(mapManager, e, g, costTable[e[1]], false)); }},
			{"Pirate", [&]() { characters.push_back(std::make_shared<Pirate>(mapManager, e, g, costTable[e[1]], false)); }}
		};
	
		
		if (characterFactory.find(e[1]) != characterFactory.end()) {
			characterFactory[e[1]]();
			characters.back()->setTileAnimation();
		} else {
			std::cerr << "Unknown enemy type: " << e[1] << std::endl;
		}
		
	}
	LOG_INFO("Enemy loading success.");
}

void EnemyManager::setInitialLevel(int level){
	std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile( DATA_CHARACTER "enemy/enemy_starting_position.csv");
	
	data->erase(data->begin());
	for(auto &e: *data){
		for(auto &c: characters){
			if(c->getName() == e[0] && c->getClassName() == e[1]){
				if(c->getAbsolutePos()!=glm::ivec2(0,0)){
					characters.push_back(c->clone());

					if (e[level*2] != "X"){
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
					c->setAbsolutePos({std::stoi(e[level*2]) * TILE_SIZE, std::stoi(e[level*2+1]) * TILE_SIZE});
					LOG_INFO("Set " + c->getName() + " Position: " + std::to_string(c->getAbsolutePos().x)+ ", " + std::to_string(c->getAbsolutePos().y));
				}
				else{
					c->setAbsolutePos({-1, -1});
					c->setVisible(false);
				}
				break;
			}

		}
	}
}
