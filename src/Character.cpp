#include "Character.hpp"

Character::Character(
	std::vector<std::string> n_list,
	std::vector<std::string> g_list,
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer
	): walkCost(wc), isPlayer(isPlayer) {

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

void Character::update(){
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
					m_Transform.scale = {-1, 1};
					break;
				case Forword::Right:
					m_Drawable = walkAnimation[Forword::Right];
					m_Transform.scale = {1, 1};
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

void Character::setTileAnimation(){
	
	std::string id = className;
	id[0] = std::tolower(id[0]);

	std::vector<std::string> reg = {};
	for (int i = 0; i <= 4 * gapOfAnimation - 1; i++) {
		reg.push_back(ASSETS TILES_PLAYER_FOLDER + id + "_" + std::to_string(i) + ".png");
		LOG_INFO(ASSETS TILES_PLAYER_FOLDER + id + "_" + std::to_string(i) + ".png");

		if (i == gapOfAnimation - 1) {
			standAnimation = std::make_shared<Util::Animation>(reg, true, TILE_INTERVAL, true, 0);
			reg = {};
		} else if (i == gapOfAnimation + 1) {
			walkAnimation[Forword::Down] = std::make_shared<Util::Animation>(reg, true, TILE_INTERVAL, true, 0);
			reg = {};
		} else if (i == gapOfAnimation + 3) {
			walkAnimation[Forword::Left] = std::make_shared<Util::Animation>(reg, true, TILE_INTERVAL, true, 0);
			reg = {};
		} else if (i == gapOfAnimation + 5) {
			walkAnimation[Forword::Up] = std::make_shared<Util::Animation>(reg, true, TILE_INTERVAL, true, 0);
			reg = {};
		} else if (i == gapOfAnimation * 4 - 1) {
			waitAnimation = std::make_shared<Util::Animation>(reg, true, TILE_INTERVAL, true, 0);
			reg = {};
		}
	}
	

	m_Transform.scale = {TILE_SCALE, TILE_SCALE};
	m_Visible = true;
	m_ZIndex = 2;
}

Lord::Lord(
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(n_list, g_list, wc, isPlayer){
}

Cavalier::Cavalier(
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(n_list, g_list, wc, isPlayer){
}

Paladin::Paladin(
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(n_list, g_list, wc, isPlayer){
}

PegasusKnight::PegasusKnight(
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(n_list, g_list, wc, isPlayer){
}

Knight::Knight(
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(n_list, g_list, wc, isPlayer){
}

Archer::Archer(
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(n_list, g_list, wc, isPlayer){
	gapOfAnimation = 2;
}

// Thief::Thief(
// 	std::vector<std::string> n_list,
// 	std::vector<std::string> g_list, 
// 	std::shared_ptr<std::unordered_map<std::string, int>> wc,
// 	bool isPlayer = true
// ) : Character(n_list, g_list, wc, isPlayer){
// }

Curate::Curate(
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(n_list, g_list, wc, isPlayer){
}

Mercenary::Mercenary(
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(n_list, g_list, wc, isPlayer){
}

Fighter::Fighter(
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(n_list, g_list, wc, isPlayer){
	gapOfAnimation = 2;
}

Hunter::Hunter(
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(n_list, g_list, wc, isPlayer){
}

Pirate::Pirate(
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer = true
) : Character(n_list, g_list, wc, isPlayer){
	gapOfAnimation = 2;
}

CharacterManager::CharacterManager(int level = 1){
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
	setInitialLevel(level);
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
			{"Lord", [&]() { characters.push_back(std::make_shared<Lord>(e, g, costTable[e[1]], true)); }},
			{"Cavalier", [&]() { characters.push_back(std::make_shared<Cavalier>(e, g, costTable[e[1]], true)); }},
			{"Paladin", [&]() { characters.push_back(std::make_shared<Paladin>(e, g, costTable[e[1]], true)); }},
			{"PegasusKnight", [&]() { characters.push_back(std::make_shared<PegasusKnight>(e, g, costTable[e[1]], true)); }},
			{"Archer", [&]() { characters.push_back(std::make_shared<Archer>(e, g, costTable[e[1]], true)); }},
			{"Knight", [&]() { characters.push_back(std::make_shared<Knight>(e, g, costTable[e[1]], true)); }},
			//{"Thief", [&]() { characters.push_back(std::make_shared<Thief>(e, g, costTable[e[1]], true)); }},
			{"Curate", [&]() { characters.push_back(std::make_shared<Curate>(e, g, costTable[e[1]], true)); }},
			{"Mercenary", [&]() { characters.push_back(std::make_shared<Mercenary>(e, g, costTable[e[1]], true)); }},
			{"Fighter", [&]() { characters.push_back(std::make_shared<Fighter>(e, g, costTable[e[1]], true)); }},
			{"Hunter", [&]() { characters.push_back(std::make_shared<Hunter>(e, g, costTable[e[1]], true)); }},
			{"Pirate", [&]() { characters.push_back(std::make_shared<Pirate>(e, g, costTable[e[1]], true)); }}
		};
	
		
		if (characterFactory.find(e[1]) != characterFactory.end()) {
			characterFactory[e[1]]();
			characters.back()->setTileAnimation();
			characters.back()->update();
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

std::vector<std::shared_ptr<CameraGameObject>> CharacterManager::getChildren(){
	std::vector<std::shared_ptr<CameraGameObject>> children = {};
	for(auto &c: characters)children.push_back(c);
	return children;
}