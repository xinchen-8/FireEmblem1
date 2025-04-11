#include "Character.hpp"

Character::Character(
	std::shared_ptr<MapManager> mm,
	std::vector<std::string> n_list,
	std::vector<std::string> g_list,
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer
	): mapManager(mm), walkCost(wc), isPlayer(isPlayer) {

	name = n_list[0];
	LOG_INFO("load data for character: " + name); 
	className = n_list[1];

	Lv = std::stoi(n_list[2]); Ex = std::stoi(n_list[3]);
	HP = std::stoi(n_list[4]); Str = std::stoi(n_list[5]);
	Skl = std::stoi(n_list[6]); Wlv = std::stoi(n_list[7]);
	Spd = std::stoi(n_list[8]); Lck = std::stoi(n_list[9]);
	Def = std::stoi(n_list[10]); Res = std::stoi(n_list[11]);
	Mov = std::stoi(n_list[12]);

	HPGR = std::stoi(g_list[1]); StrGR = std::stoi(g_list[2]);
	SklGR = std::stoi(g_list[3]); WlvGR = std::stoi(g_list[4]);
	SpdGR = std::stoi(g_list[5]); LckGR = std::stoi(g_list[6]);
	DefGR = std::stoi(g_list[7]); ResGR = std::stoi(g_list[8]);
}

void Character::setAnimation(){
	switch(status){
		case CharacterStatus::Normal:
			m_Drawable = standAnimation;
			break;
		case CharacterStatus::Moving:
			switch(forword){
				case Forword::Up:
					m_Drawable = walkAnimation[Forword::Up];
					break;
				case Forword::Down:
					m_Drawable = walkAnimation[Forword::Down];
					break;
				case Forword::Left:
					m_Drawable = walkAnimation[Forword::Right];
					m_Transform.scale.x = -TILE_SCALE;
					break;
				case Forword::Right:
					m_Drawable = walkAnimation[Forword::Right];
					break;
			}
			break;
		case CharacterStatus::Waiting:
			m_Drawable = waitAnimation;
			break;
		default:
			break;
	}
}

void Character::refreshMoveRange(){
	// std::cout<<name<<" refresh move range."<<std::endl;
	moveRange.clear();
	findMoveRange(Mov+1, getAbsolutePos());
}

void Character::setStatus(CharacterStatus status){
	this->status = status;
	setAnimation();
}

void Character::setTileAnimation(){
	
	std::string id = className;
	id[0] = std::tolower(id[0]);

	std::vector<std::string> reg = {};
	for (int i = 0; i <= 4 * gapOfAnimation - 1; i++) {
		reg.push_back(ASSETS TILES_PLAYER_FOLDER + id + "_" + std::to_string(i) + ".png");
		LOG_INFO("add " ASSETS TILES_PLAYER_FOLDER + id + "_" + std::to_string(i) + ".png");

		if (i == gapOfAnimation - 1) {
			standAnimation = std::make_shared<Util::Animation>(reg, true, TILE_INTERVAL, true, 0);
			reg = {};
		}
		else if (i == gapOfAnimation + 1) {
			walkAnimation[Forword::Down] = std::make_shared<Util::Animation>(reg, true, TILE_INTERVAL, true, 0);
			reg = {};
		}
		else if (i == gapOfAnimation + 3) {
			walkAnimation[Forword::Right] = std::make_shared<Util::Animation>(reg, true, TILE_INTERVAL, true, 0);
			reg = {};
		}
		else if (i == gapOfAnimation + 5) {
			walkAnimation[Forword::Up] = std::make_shared<Util::Animation>(reg, true, TILE_INTERVAL, true, 0);
			reg = {};
		}
		else if (i == gapOfAnimation * 4 - 1) {
			waitAnimation = std::make_shared<Util::Animation>(reg, true, TILE_INTERVAL, true, 0);
			reg = {};
		}
	}
	m_Transform.scale = {TILE_SCALE, TILE_SCALE};
	m_Visible = true;
	m_ZIndex = 3;
	setAnimation();
}

void Character::findMoveRange(int mov, glm::vec2 a_pos){
	if (mov < 0) return;

	auto it = moveRange.find(a_pos);
	if (it != moveRange.end() && it->second >= mov) return;

    std::shared_ptr<Tile> tile = mapManager->getPosTile(a_pos);
    if (!tile) return;

    int cost = (*walkCost)[tile->getName()];
	if(cost==-1) return;
	if(cost==0) cost = (*walkCost)["Default"];
    int new_mov = mov - cost;
    if(new_mov <= 0) return;

	moveRange[a_pos] = mov;
    // std::cout << new_mov << ": add " << a_pos.x << ", " << a_pos.y << std::endl;
    
    findMoveRange(new_mov, a_pos + glm::vec2(TILE_SIZE, 0));
    findMoveRange(new_mov, a_pos + glm::vec2(-TILE_SIZE, 0));
    findMoveRange(new_mov, a_pos + glm::vec2(0, TILE_SIZE));
    findMoveRange(new_mov, a_pos + glm::vec2(0, -TILE_SIZE));
}

Lord::Lord(
	std::shared_ptr<MapManager> mm,
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(mm, n_list, g_list, wc, isPlayer){
}

Cavalier::Cavalier(
	std::shared_ptr<MapManager> mm,
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(mm, n_list, g_list, wc, isPlayer){
}

Paladin::Paladin(
	std::shared_ptr<MapManager> mm,
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(mm, n_list, g_list, wc, isPlayer){
}

PegasusKnight::PegasusKnight(
	std::shared_ptr<MapManager> mm,
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(mm, n_list, g_list, wc, isPlayer){
}

Knight::Knight(
	std::shared_ptr<MapManager> mm,
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(mm, n_list, g_list, wc, isPlayer){
}

Archer::Archer(
	std::shared_ptr<MapManager> mm,
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(mm, n_list, g_list, wc, isPlayer){
	gapOfAnimation = 2;
}

// Thief::Thief(
// 	std::vector<std::string> n_list,
// 	std::vector<std::string> g_list, 
// 	std::shared_ptr<std::unordered_map<std::string, int>> wc,
// 	bool isPlayer = true
// ) : Character(mm, n_list, g_list, wc, isPlayer){
// }

Curate::Curate(
	std::shared_ptr<MapManager> mm,
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(mm, n_list, g_list, wc, isPlayer){
}

Mercenary::Mercenary(
	std::shared_ptr<MapManager> mm,
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(mm, n_list, g_list, wc, isPlayer){
}

Fighter::Fighter(
	std::shared_ptr<MapManager> mm,
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(mm, n_list, g_list, wc, isPlayer){
	gapOfAnimation = 2;
}

Hunter::Hunter(
	std::shared_ptr<MapManager> mm,
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(mm, n_list, g_list, wc, isPlayer){
}

Pirate::Pirate(
	std::shared_ptr<MapManager> mm,
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(mm, n_list, g_list, wc, isPlayer){
	gapOfAnimation = 2;
}

CharacterManager::CharacterManager(std::shared_ptr<MapManager> mm) : mapManager(mm) {

	std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile(ASSETS CHARACTER_FOLDER "class_cost.csv");
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
	std::shared_ptr<Tile> tip = std::make_shared<Tile>("movable", ASSETS SELECTION_FOLDER "tip0.png");
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

void CharacterManager::loadCharacter(){
	std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile(ASSETS CHARACTER_FOLDER "players/player_base_data.csv");
	std::shared_ptr<std::vector<std::vector<std::string>>> g_data = Tool::inputFile(ASSETS CHARACTER_FOLDER "players/player_growth_rate.csv");
	if(!data) LOG_ERROR("Character loading failed!");
	
	data->erase(data->begin());
	g_data->erase(g_data->begin());
	for(std::size_t i = 0; i<data->size(); i++){
		std::vector<std::string> e = (*data)[i];
		std::vector<std::string> g = (*g_data)[i];

		std::unordered_map<std::string, std::function<void()>> characterFactory = {
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
			characters.back()->refreshMoveRange();
		} else {
			std::cerr << "Unknown character type: " << e[1] << std::endl;
		}
		
	}
	LOG_INFO("Character loading success.");
}

void CharacterManager::setInitialLevel(int level){
	std::shared_ptr<std::vector<std::vector<std::string>>> data = Tool::inputFile(ASSETS CHARACTER_FOLDER "players/player_starting_position.csv");
	
	data->erase(data->begin());
	for(auto &e: *data){
		for(auto &c: characters){
			
			if(c->getName() == e[0]){
				if (e[level] != "X"){
					c->setAbsolutePos({std::stoi(e[level]) * TILE_SIZE, std::stoi(e[level+1]) * TILE_SIZE});
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

void CharacterManager::changeTipsVisible(){
	tipsVisible = !tipsVisible;
}

std::unordered_map<glm::vec2, int> CharacterManager::selectCharacter(glm::vec2 a_pos){
	for(auto &c: characters){
		if(c->getAbsolutePos() == a_pos){
			c->refreshMoveRange();

			for(auto [pos, mov]: c->getMoveRange()){
				// std::cout<<"meow " << e.x << ", " << e.y << std::endl;
				std::shared_ptr<Tile> tip = getTipTile(pos);
				
				tip->setStart();
				std::vector<std::string> r = {ASSETS SELECTION_FOLDER "tip0.png"};
				tip->setAnimation( std::make_shared<Util::Animation>(
					r, true, TILE_INTERVAL, true, 0)
				);
				tip->SetVisible(tipsVisible);
				// std::cout<<"meowwwwww"<<std::endl;

			}


			c->setStatus(CharacterStatus::Moving);
			return c->getMoveRange();
		}
	}
	return {};
}

std::shared_ptr<Character> CharacterManager::getCharacter(std::string id){
	for(auto &c: characters){
		if(c->getName() == id) return c;
	}
	return nullptr;
}

std::shared_ptr<Tile> CharacterManager::getTipTile(glm::vec2 a_pos){
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