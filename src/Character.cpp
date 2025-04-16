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

void Character::walkDirectly(){
	if(status != CharacterStatus::Moving) return;
	if(walkPath.size() == 0) return;

	std::cout<<"walkDirectly: " << absolutePos.x << ", " << absolutePos.y << std::endl;
	
	glm::ivec2 p = walkPath.front();
	if(p==absolutePos){
		walkPath.pop();
		if(!walkPath.size()){
			setStatus(CharacterStatus::Normal);
			return;
		}
		else p = walkPath.front();
	}
	if(p.x > absolutePos.x) forword = Forword::Right;
	else if(p.x < absolutePos.x) forword = Forword::Left;
	else if(p.y < absolutePos.y) forword = Forword::Down;
	else if(p.y > absolutePos.y) forword = Forword::Up;
	setAnimation();

	glm::ivec2 delta = p - absolutePos;
	glm::ivec2 step{0,0};
	if (delta.x != 0) step.x = (delta.x > 0 ? 8 : -8); //64 must be divisible by 8
	if (delta.y != 0) step.y = (delta.y > 0 ? 8 : -8); //64 must be divisible by 8
	absolutePos += step;
	
	if(!walkPath.size()){
		setStatus(CharacterStatus::Normal);
	}
}


void Character::buildWalkPath(glm::ivec2 a_pos){
	if (moveRange.find(a_pos) == moveRange.end()){
		LOG_ERROR("Invalid move range: " + std::to_string(a_pos.x) + ", " + std::to_string(a_pos.y));
		return;
	}

//Dijkstra's algorithm
	//initialize
	std::shared_ptr<Tile> start = mapManager->getPosTile(getAbsolutePos());
	std::shared_ptr<Tile> end = mapManager->getPosTile(a_pos);
	std::vector<glm::ivec2> directions = {
        {TILE_SIZE, 0}, {0, TILE_SIZE}, {-TILE_SIZE, 0}, {0, -TILE_SIZE}
    };
	
	for(auto &p: moveRange) p.second = INT_MAX;
    std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, std::greater<DijkstraNode>> pq;
	moveRange[start->getAbsolutePos()] = 0;
	std::queue<glm::ivec2> p;
	p.push(start->getAbsolutePos());
	pq.push({0, start->getAbsolutePos(), p}); //first node
	
	//main
    while (!pq.empty()) {
        DijkstraNode cur = pq.top(); pq.pop();

        if (cur.pos == end->getAbsolutePos()) {
			std::cout<<"find path: " << cur.cost << std::endl;
			std::cout<<"path: ";

			std::queue<glm::ivec2> curPath = cur.path;
			while(!curPath.empty()){
				glm::ivec2 p = curPath.front(); curPath.pop();
				std::cout<<"("<<p.x<<", "<<p.y<<") ";
			}
			std::cout<<std::endl;

			walkPath = cur.path;
        }

        for (auto& d : directions) {
            glm::ivec2 next = cur.pos + d;
			
			//judge border of range
            if (moveRange.find(next) == moveRange.end()) continue;
			std::cout<<"next: "<<next.x<<", "<<next.y<<std::endl;
            auto nextTile = mapManager->getPosTile(next);
            if (!nextTile) continue;

            int tileCost = (*walkCost)[nextTile->getName()];
            if (tileCost == 0) tileCost = (*walkCost)["Default"];
            int newCost = cur.cost + tileCost;

            if (newCost < moveRange[next]) {
                moveRange[next] = newCost;
                auto newPath = cur.path;
                newPath.push(next);
                pq.push({newCost, next, newPath});
            }
        }
    }
}

void Character::refreshMoveRange(){
	// std::cout<<name<<" refresh move range."<<std::endl;
	moveRange.clear();
	
    std::shared_ptr<Tile> tile = mapManager->getPosTile(absolutePos);
    int cost = (*walkCost)[tile->getName()];
	if(cost==0) cost = (*walkCost)["Default"];
	findMoveRange(Mov+cost, absolutePos);
}

void Character::setAnimation(){
	switch(status){
		case CharacterStatus::Normal:
			forword = Forword::Down;
			m_Drawable = standAnimation;
			m_Transform.scale.x = TILE_SCALE;
			break;
		case CharacterStatus::Moving:
			switch(forword){
				case Forword::Up:
					m_Drawable = walkAnimation[Forword::Up];
					m_Transform.scale.x = TILE_SCALE;
					break;
				case Forword::Down:
					m_Drawable = walkAnimation[Forword::Down];
					m_Transform.scale.x = TILE_SCALE;
					break;
				case Forword::Left:
					m_Drawable = walkAnimation[Forword::Right];
					m_Transform.scale.x = -TILE_SCALE;
					break;
				case Forword::Right:
					m_Drawable = walkAnimation[Forword::Right];
					m_Transform.scale.x = TILE_SCALE;
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

void Character::setStatus(CharacterStatus status){
	this->status = status;
	std::cout<<"set status "<< std::endl;

	setAnimation();
}

void Character::setTileAnimation(){
	
	std::string id = className;
	id[0] = std::tolower(id[0]);


	std::vector<std::string> reg = {};
	for (int i = 0; i <= 4 * gapOfAnimation - 1; i++) {
		if(isPlayer){
			reg.push_back(TILE_PLAYER + id + "_" + std::to_string(i) + ".png");
			LOG_INFO("add " TILE_PLAYER + id + "_" + std::to_string(i) + ".png");
		}
		else{
			reg.push_back(TILE_ENEMY + id + "_" + std::to_string(i) + ".png");
			LOG_INFO("add " TILE_ENEMY + id + "_" + std::to_string(i) + ".png");
		}

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

void Character::findMoveRange(int mov, glm::ivec2 a_pos){
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
    
    findMoveRange(new_mov, a_pos + glm::ivec2(TILE_SIZE, 0));
    findMoveRange(new_mov, a_pos + glm::ivec2(-TILE_SIZE, 0));
    findMoveRange(new_mov, a_pos + glm::ivec2(0, TILE_SIZE));
    findMoveRange(new_mov, a_pos + glm::ivec2(0, -TILE_SIZE));
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

Thief::Thief(std::shared_ptr<MapManager> mm,
	std::vector<std::string> n_list,
	std::vector<std::string> g_list, 
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer) : Character(mm, n_list, g_list, wc, isPlayer){
}

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
