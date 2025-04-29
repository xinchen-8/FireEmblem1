#include "Item.hpp"

Item::Item(std::string name, int uses, int worth, std::string note) : 
    name(name), uses(uses), worth(worth), note(note){

}

Vulnerary::Vulnerary(std::vector<std::string> i_list) : 
    Item(i_list[ITEM_INDEX::NAME], std::stoi(i_list[ITEM_INDEX::USES]),
         std::stoi(i_list[ITEM_INDEX::WORTH]), i_list[ITEM_INDEX::NOTE]){

}

void Vulnerary::use(Character* user, std::shared_ptr<Character> target){
    user->setHP(user->getCurHP()+10);
    uses--;
}

HandHeldItem::HandHeldItem(std::vector<std::string> w_list): 
    Item(w_list[HANDHELD_INDEX::NAME], std::stoi(w_list[HANDHELD_INDEX::USES]), 
        std::stoi(w_list[HANDHELD_INDEX::WORTH]), w_list[HANDHELD_INDEX::NOTE]){
    
    mt     = std::stoi(w_list[HANDHELD_INDEX::MT]);
	wt     = std::stoi(w_list[HANDHELD_INDEX::WT]);
	hit    = std::stoi(w_list[HANDHELD_INDEX::HIT]);

    std::string data = w_list[HANDHELD_INDEX::RNG];
    size_t pos = w_list[HANDHELD_INDEX::RNG].find("~");

    if (pos != std::string::npos) {
        int first = std::stoi(data.substr(0, pos));
        int second = std::stoi(data.substr(pos + 1));
        rng.push_back(first);
        rng.push_back(second);
    } else {
        rng.push_back(std::stoi(data));
    }

	wlv    = std::stoi(w_list[HANDHELD_INDEX::WLV]);
	crt	   = std::stoi(w_list[HANDHELD_INDEX::CRT]);
}

void HandHeldItem::use(Character* user, std::shared_ptr<Character> target){
    int power = user->getStr() + mt * (isAgainst(target->getClassName())? 3 : 1);
	int critical = (user->getSkl() + user->getLck()) / 2 + crt;
	int accuracy = hit + user->getSkl();
    if(target->attacked(power, critical, accuracy)) uses--;
}

bool HandHeldItem::isAgainst(std::string className){
	std::cout<<"meowstr"<<std::endl;
    for(auto &a: against){
	    std::cout<<"meowstr1"<<std::endl;
        if(a==className) return true;
    }
    return false;
}

Rapier::Rapier(std::vector<std::string> w_list) : HandHeldItem(w_list) {
    against = { "Cavalier", "Knight", "Paladin", "General" };
}

IronSword::IronSword(std::vector<std::string> w_list) : HandHeldItem(w_list) {

}

SteelSword::SteelSword(std::vector<std::string> w_list) : HandHeldItem(w_list) {

}

IronLance::IronLance(std::vector<std::string> w_list) : HandHeldItem(w_list) {

}

SilverLance::SilverLance(std::vector<std::string> w_list) : HandHeldItem(w_list) {

}

Javelin::Javelin(std::vector<std::string> w_list) : HandHeldItem(w_list) {

}

IronBow::IronBow(std::vector<std::string> w_list) : HandHeldItem(w_list) {

}

SteelBow::SteelBow(std::vector<std::string> w_list) : HandHeldItem(w_list) {

}

Bowgun::Bowgun(std::vector<std::string> w_list) : HandHeldItem(w_list) {

}

Heal::Heal(std::vector<std::string> w_list) : HandHeldItem(w_list) {

}

void Heal::use(Character* user, std::shared_ptr<Character> target) {
    int reg = target->getCurHP();
    target->setHP(reg+10);
    LOG_INFO(target->getName()+" HP+10: " + std::to_string(reg) + " -> " + std::to_string(target->getCurHP()));
}

IronAxe::IronAxe(std::vector<std::string> w_list) : HandHeldItem(w_list) {

}

SteelAxe::SteelAxe(std::vector<std::string> w_list) : HandHeldItem(w_list) {

}

HandAxe::HandAxe(std::vector<std::string> w_list) : HandHeldItem(w_list) {

}

Hammer::Hammer(std::vector<std::string> w_list) : HandHeldItem(w_list) {
    against = { "Knight", "General" };
}
