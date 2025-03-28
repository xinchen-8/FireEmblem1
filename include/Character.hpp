/*
#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include "Integration.hpp"

class Character : public Util::GameObject {
public:
	Character(
		std::vector<std::string> n_list, 
		std::shared_ptr<std::unordered_map> wc
	);
	//items
private:
	std::string name = "";

	int Lv = 0; int Ex = 0;
	int HP = 0; int Str = 0;
	int Skl = 0; int Wlv = 0;
	int Spd = 0; int Lck = 0;
	int Def = 0; int Red = 0;
	int Mov = 0;

	std::shared_ptr<Util::Animation> headshotAnimation = nullptr;
	std::shared_ptr<Util::Animation> standAnimation = nullptr;
	std::shared_ptr<Util::Animation> walkAnimation = nullptr;

	std::shared_ptr<std::unordered_map> walkCost = nullptr;

};


class Lord : public Character {
	
};


class PegasusKnight : public Character {

};


class Paladin : public Character {

};
	

class Cavalier : public Character {

};


class Knight : public Character {

};

	
class Archer : public Character {

};


class Curate : public Character {

};


class Mercenary : public Character {

};


class Fighter : public Character {

};


class Hunter : public Character {

};


class Pirate : public Character {

};

#endif
*/