#include "Character/Character.hpp"

Character::Character(
	std::shared_ptr<MapManager> mm,
	std::vector<std::string> n_list,
	std::vector<std::string> g_list,
	std::shared_ptr<std::unordered_map<std::string, int>> wc,
	bool isPlayer
	): mapManager(mm), walkCost(wc), isPlayer(isPlayer), items(4, nullptr) {

	name = n_list[CHARACTER_INDEX::NAME];
	className = n_list[CHARACTER_INDEX::CLASS];
	LOG_INFO("load data for character: " + name); 

	Lv = std::stoi(n_list[CHARACTER_INDEX::LV]);              Ex = std::stoi(n_list[CHARACTER_INDEX::EX]);
	Hp_Limit = std::stoi(n_list[CHARACTER_INDEX::HP]);        Hp_Current = Hp_Limit;
	Str = std::stoi(n_list[CHARACTER_INDEX::STR]);            Skl = std::stoi(n_list[CHARACTER_INDEX::SKL]);
	Wlv = std::stoi(n_list[CHARACTER_INDEX::WLV]);            Spd = std::stoi(n_list[CHARACTER_INDEX::SPD]);
	Lck = std::stoi(n_list[CHARACTER_INDEX::LCK]);            Def = std::stoi(n_list[CHARACTER_INDEX::DEF]);
	Res = std::stoi(n_list[CHARACTER_INDEX::RES]);            Mov = std::stoi(n_list[CHARACTER_INDEX::MOV]);

	HPGR = std::stoi(g_list[CHARACTER_GROWTH_INDEX::HP]);     StrGR = std::stoi(g_list[CHARACTER_GROWTH_INDEX::STR]);
	SklGR = std::stoi(g_list[CHARACTER_GROWTH_INDEX::SKL]);   WlvGR = std::stoi(g_list[CHARACTER_GROWTH_INDEX::WLV]);
	SpdGR = std::stoi(g_list[CHARACTER_GROWTH_INDEX::SPD]);   LckGR = std::stoi(g_list[CHARACTER_GROWTH_INDEX::LCK]);
	DefGR = std::stoi(g_list[CHARACTER_GROWTH_INDEX::DEF]);   ResGR = std::stoi(g_list[CHARACTER_GROWTH_INDEX::RES]);

	if(className=="Archer" || className=="Fighter" || className=="Pirate")
		gapOfAnimation = 2;
	// sword
	if(className=="Lord" || className=="PegasusKnight" || className=="Paladin" || 
		className=="Cavalier" || className=="Knight" || className=="Thief" || className=="Mercenary")

		usableHandHeldItem.push_back(HandHeldItemType::Sword);
	// lance
	if(className=="PegasusKnight" || className=="Paladin" || className=="Cavalier" || className=="Knight")
		usableHandHeldItem.push_back(HandHeldItemType::Lance);
	// bow
	if(className=="Archer" || className=="Hunter")
		usableHandHeldItem.push_back(HandHeldItemType::Bow);
	// staff
	if(className=="Curate")
		usableHandHeldItem.push_back(HandHeldItemType::Staff);
	//axe
	if(className=="Fighter" || className=="Pirate")
		usableHandHeldItem.push_back(HandHeldItemType::Axe);

	if(className=="Archer" || className=="Fighter" || className=="Pirate")
		gapOfAnimation = 2;
	
	m_ZIndex = 3;
}

bool Character::walkDirectly(){
	if(status != CharacterStatus::Moving) return false;
	if(walkPath.size() == 0) return false;

	// std::cout<<"walkDirectly: " << absolutePos.x << ", " << absolutePos.y << std::endl;
	
	glm::ivec2 p = walkPath.front();
	if(p==absolutePos){
		walkPath.pop();
		if(!walkPath.size()){
			setStatus(CharacterStatus::Normal);
			return true;
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
	return false;
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
			// std::cout<<"next: "<<next.x<<", "<<next.y<<std::endl;
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
	m_ZIndex = 4;
}

void Character::refreshMoveRange(std::unordered_set<glm::ivec2> mask){
	moveRange.clear();
	attackRange.clear();
	
    std::shared_ptr<Tile> tile = mapManager->getPosTile(absolutePos);
    int cost = (*walkCost)[tile->getName()];
	if(cost==0) cost = (*walkCost)["Default"];
	findMoveRange(Mov+cost, absolutePos, mask);
	findAttackRange();
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

void Character::clearWalkPath() {
	while(walkPath.size()) walkPath.pop();
	m_ZIndex = 3;
}

void Character::resetRange(){
	moveRange.clear();
	attackRange.clear();
	moveRange[absolutePos] = 0;
}

void Character::setStatus(CharacterStatus status){
	this->status = status;
	setAnimation();
}

void Character::setHandHeldItemWithIndex(int index){
	if(std::dynamic_pointer_cast<HandHeldItem>(items[index])){
		handheld_index = index;
		LOG_INFO("The Character Hand Holding a " + items[index]->getName());
	}
	else LOG_ERROR("This Index of Items is not a HandHeld Item.");
}

void Character::setTileAnimation(){
	
	std::string id = className;
	id[0] = std::tolower(id[0]);


	std::vector<std::string> reg = {};
	for (int i = 0; i < 2*3 + gapOfAnimation*2; i++) {
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
		else if (i == 2*3 + gapOfAnimation*2-1) {
			waitAnimation = std::make_shared<Util::Animation>(reg, true, TILE_INTERVAL, true, 0);
			reg = {};
		}
	}
	m_Transform.scale = {TILE_SCALE, TILE_SCALE};
	m_Visible = true;
	setAnimation();
}

void Character::useVulnerary(int index){
	std::shared_ptr<Item> item = items[index];
	std::shared_ptr<Vulnerary> v = std::dynamic_pointer_cast<Vulnerary>(item);
	if(v) v->use(this, nullptr);
}

bool Character::pushItem(std::shared_ptr<Item> item){
	for (auto& slot : items) {
		if (!slot) { 
			slot = item;
			LOG_INFO("Add "+item->getName()+" Success.");
			return true;
		}
		std::cout<<slot->getName()<<std::endl;
	}
	LOG_WARN("Add "+item->getName()+" Unsuccess.");
	return false;
}

void Character::freshItem(int delete_index){
	if(delete_index!=-1) items.erase(items.begin() + delete_index);
	for(int i=0; i<items.size(); i++){
		std::shared_ptr<HandHeldItem> handHeldItem = 
			std::dynamic_pointer_cast<HandHeldItem>(items[i]);
		
		if(handHeldItem){
			handheld_index = i;
			return;
		}
	}
}

void Character::setHP(int hp) {
	if(hp > Hp_Limit) Hp_Current = Hp_Limit;
	else if(hp > 0) Hp_Current = hp;
}

bool Character::attacked(int power, int crt, int acc, bool isMagical){ // return isnotAvoid

	if(isMagical){
		// not yet (character1, 2 have no magical attack)
		//if(rand() % 100) < lck)) return true;
		//power -= Res;
	}
	else{
		if((rand() % 100) < acc-(mapManager->getPosTile(absolutePos)->getAvoid()+Spd)){ // for accuracy
			power -= Def;
			power *= ((rand() % 100) < crt)? 3 : 1;
			Hp_Current -= power;
			LOG_INFO(name + "was attacked: HP-" + std::to_string(power));
			Hp_Current = (Hp_Current<0)? 0: Hp_Current;
			return true;
		}
		else return false;
	}
}

bool Character::attack(std::shared_ptr<Character> target){
	std::shared_ptr<Item> item = getCurrentHandHeldItem();

	if(!item){
		LOG_ERROR(name + " have no handheld weapon.");
		return false;
	}
	std::shared_ptr<HandHeldItem> handHeldItem = std::dynamic_pointer_cast<HandHeldItem>(item);	
	return handHeldItem->use(this, target);
}

std::shared_ptr<HandHeldItem> Character::getCurrentHandHeldItem(){
	if(!items[handheld_index]) freshItem();
	else return  std::dynamic_pointer_cast<HandHeldItem>(items[handheld_index]);
	return nullptr;
}

void Character::findMoveRange(int mov, glm::ivec2 a_pos, std::unordered_set<glm::ivec2> mask){
	if (mov < 0) return;

	//the best way
	auto p = moveRange.find(a_pos);
	if (p != moveRange.end() && p->second >= mov) return;
	
	//enemy pos and locked tiles of map
	auto m = mask.find(a_pos);
	if (m != mask.end()) return;

    std::shared_ptr<Tile> tile = mapManager->getPosTile(a_pos);
    if (!tile) return;

    int cost = (*walkCost)[tile->getName()];
	if(cost==-1) return;
	if(cost==0) cost = (*walkCost)["Default"];
    int new_mov = mov - cost;
    if(new_mov <= 0) return;

	moveRange[a_pos] = mov;
    // std::cout << new_mov << ": add " << a_pos.x << ", " << a_pos.y << std::endl;
    
    findMoveRange(new_mov, a_pos + glm::ivec2(TILE_SIZE, 0), mask);
    findMoveRange(new_mov, a_pos + glm::ivec2(-TILE_SIZE, 0), mask);
    findMoveRange(new_mov, a_pos + glm::ivec2(0, TILE_SIZE), mask);
    findMoveRange(new_mov, a_pos + glm::ivec2(0, -TILE_SIZE), mask);
}

void Character::findAttackScope(){
	moveRange.clear();
	attackRange.clear();
	moveRange[absolutePos] = 0;

	std::set<int> scope;
    for (std::shared_ptr<Item>& i : items) {
		if(!i) continue;
		std::shared_ptr<HandHeldItem> hhi = std::dynamic_pointer_cast<HandHeldItem>(i);
        for (int& j : hhi->getRng()) scope.insert(j);
    }

    if (scope.empty())
        return;

    glm::ivec2 dirs[] = {
        {TILE_SIZE, 0}, {-TILE_SIZE, 0},
        {0, TILE_SIZE}, {0, -TILE_SIZE}
    };

    std::queue<std::pair<glm::ivec2, int>> q;
    std::unordered_set<glm::ivec2> visited;

    for (const auto& pos : moveRange) {
        for (const auto& dir : dirs) {
            glm::ivec2 next = pos.first + dir;
            if (mapManager->getPosTile(next)) {
                q.push({next, 1});
            }
        }
    }

    while (!q.empty()) {
        auto [pos, steps] = q.front(); q.pop();

        if (visited.find(pos) != visited.end())
            continue;
        if (moveRange.find(pos) != moveRange.end())
            continue; 

        visited.insert(pos);

        if (scope.count(steps)) {
            attackRange[pos] = steps; 
            // std::cout << "RED(" << pos.x << ", " << pos.y << ")" << std::endl;
        }

        if (*scope.rbegin() > steps) {
            for (const auto& dir : dirs) {
                glm::ivec2 next = pos + dir;
                if (mapManager->getPosTile(next)) {
                    q.push({next, steps + 1});
                }
            }
        }
    }
}

void Character::findAttackRange(){
	int atk_rng = 0;
	for(std::shared_ptr<Item> &i: items){
		std::shared_ptr<Weapon> hhi = std::dynamic_pointer_cast<Weapon>(items[handheld_index]);
		if(!hhi) continue;
		for(int &j: hhi->getRng()) if(atk_rng < j) atk_rng = j;
	}
	//BFS
	if (atk_rng <= 0) return;

	glm::ivec2 dirs[] = {
		{TILE_SIZE, 0}, {-TILE_SIZE, 0},
		{0, TILE_SIZE}, {0, -TILE_SIZE}
	};

	std::queue<std::pair<glm::ivec2, int>> q;
	std::unordered_set<glm::ivec2> visited;

	for (const auto& pos : moveRange) {
		for (const auto& dir : dirs) {
			glm::ivec2 next = pos.first + dir;
			if(mapManager->getPosTile(next))
				q.push({next, atk_rng - 1});
		}
	}

	while (!q.empty()) {
		auto [pos, atk_left] = q.front(); q.pop();

		if (atk_left < 0) continue;
		if (moveRange.find(pos) != moveRange.end()) continue;
		if (visited.find(pos) != visited.end()) continue;

		visited.insert(pos);
		attackRange[pos] = atk_left;
		//std::cout << "RED(" << pos.x << ", " << pos.y << ")" << std::endl;

		for (const auto& dir : dirs) {
			q.push({pos + dir, atk_left - 1});
		}
	}
}

std::shared_ptr<Character> Character::clone(){
	auto c = std::make_shared<Character>(*this);
	if (m_Drawable)
		c->setTileAnimation();
	c->deleteAllItems();
	for(auto &i: items) c->pushItem(i->clone());
	return c;
}