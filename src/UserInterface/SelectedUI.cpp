#include "UserInterface/SelectedUI.hpp"

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