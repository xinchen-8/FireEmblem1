#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include "Integration.hpp"
#include "Tile.hpp"

class Character : public CameraGameObject {
public:
	Character(
		std::vector<std::string> n_list,
		std::vector<std::string> g_list,
		std::shared_ptr<std::unordered_map<std::string, int>> wc,
		bool isPlayer
	);
	void update();
	void setStatus(CharacterStatus status){ this->status = status; }
	void setVisible(bool visible){ m_Visible = visible; }
	void setHeadshotAnimation(std::shared_ptr<Util::Animation> a_headshotAnimation){ headshotAnimation = a_headshotAnimation; }
	void setTileAnimation();
	std::string getName() { return name; }
	int getLevel() { return Lv; }
	int getExp() { return Ex; }
	int getHP() { return HP; }
	int getStr() { return Str; }
	int getSkl() { return Skl; }
	int getWlv() { return Wlv; }
	int getSpd() { return Spd; }
	int getLck() { return Lck; }
	int getDef() { return Def; }
	int getRes() { return Res; }
	int getMov() { return Mov; }
	int getHPGR() { return HPGR; }
	int getStrGR() { return StrGR; }
	int getSklGR() { return SklGR; }
	int getWlvGR() { return WlvGR; }
	int getSpdGR() { return SpdGR; }
	int getLckGR() { return LckGR; }
	int getDefGR() { return DefGR; }
	int getResGR() { return ResGR; }
	//items
protected:
	int gapOfAnimation = 3;
private:
	std::string name = "";
	std::string className = "";
	bool isPlayer = true;

	Forword forword = Forword::Left;
	CharacterStatus status = CharacterStatus::Normal;
	std::vector<WeaponType> usableWeapon = {}; 

	int Lv = 0; int Ex = 0;
	int HP = 0; int Str = 0;
	int Skl = 0; int Wlv = 0;
	int Spd = 0; int Lck = 0;
	int Def = 0; int Res = 0;
	int Mov = 0;
	
	int HPGR = 0; int StrGR = 0;
	int SklGR = 0; int WlvGR = 0;
	int SpdGR = 0; int LckGR = 0;
	int DefGR = 0; int ResGR = 0;

	std::shared_ptr<Util::Animation> headshotAnimation = nullptr;
	std::shared_ptr<Util::Animation> standAnimation = nullptr;
	std::unordered_map<Forword, std::shared_ptr<Util::Animation>> walkAnimation = {};
	std::shared_ptr<Util::Animation> waitAnimation = nullptr;

	std::shared_ptr<std::unordered_map<std::string, int>> walkCost = nullptr;

};

//class of characters
class Lord final : public Character {
public:
	Lord(
		std::vector<std::string> n_list,
		std::vector<std::string> g_list, 
		std::shared_ptr<std::unordered_map<std::string, int>> wc,
		bool isPlayer
	);
private:
	std::string className = "Lord";
	std::vector<WeaponType> usableWeapon = {
		WeaponType::Sword
	}; 
};


class PegasusKnight final : public Character {
public:
	PegasusKnight(
		std::vector<std::string> n_list,
		std::vector<std::string> g_list, 
		std::shared_ptr<std::unordered_map<std::string, int>> wc,
		bool isPlayer
	);
private:
	std::string className = "PegasusKnight";
	std::vector<WeaponType> usableWeapon = {
		WeaponType::Sword,
		WeaponType::Lance
	}; 
};


class Paladin final : public Character {
public:
	Paladin(
		std::vector<std::string> n_list,
		std::vector<std::string> g_list, 
		std::shared_ptr<std::unordered_map<std::string, int>> wc,
		bool isPlayer
	);
private:
	std::string className = "Paladin";
	std::vector<WeaponType> usableWeapon = {
		WeaponType::Sword,
		WeaponType::Lance
	}; 
};
	

class Cavalier final : public Character {
public:
	Cavalier(
		std::vector<std::string> n_list,
		std::vector<std::string> g_list, 
		std::shared_ptr<std::unordered_map<std::string, int>> wc,
		bool isPlayer
	);
private:
	std::string className = "Cavalier";
	std::vector<WeaponType> usableWeapon = {
		WeaponType::Sword,
		WeaponType::Lance
	}; 
};


class Knight final : public Character {
public:
	Knight(
		std::vector<std::string> n_list,
		std::vector<std::string> g_list, 
		std::shared_ptr<std::unordered_map<std::string, int>> wc,
		bool isPlayer
	);
private:
	std::string className = "Knight";
	std::vector<WeaponType> usableWeapon = {
		WeaponType::Sword,
		WeaponType::Lance
	}; 
};

	
class Archer final : public Character {
public:
	Archer(
		std::vector<std::string> n_list,
		std::vector<std::string> g_list, 
		std::shared_ptr<std::unordered_map<std::string, int>> wc,
		bool isPlayer
	);
private:
	std::string className = "Archer";
	std::vector<WeaponType> usableWeapon = {
		WeaponType::Bow
	}; 
};


class Curate final : public Character {
public:
	Curate(
		std::vector<std::string> n_list,
		std::vector<std::string> g_list, 
		std::shared_ptr<std::unordered_map<std::string, int>> wc,
		bool isPlayer
	);
private:
	std::string className = "Curate";
	std::vector<WeaponType> usableWeapon = {
		WeaponType::Staff
	}; 
};


class Mercenary final : public Character {
public:
	Mercenary(
		std::vector<std::string> n_list,
		std::vector<std::string> g_list, 
		std::shared_ptr<std::unordered_map<std::string, int>> wc,
		bool isPlayer
	);
private:
	std::string className = "Mercenary";
	std::vector<WeaponType> usableWeapon = {
		WeaponType::Sword
	}; 
};


class Fighter final : public Character {
public:
	Fighter(
		std::vector<std::string> n_list,
		std::vector<std::string> g_list, 
		std::shared_ptr<std::unordered_map<std::string, int>> wc,
		bool isPlayer
	);
private:
	std::string className = "Fighter";
	std::vector<WeaponType> usableWeapon = {
		WeaponType::Axe
	}; 
};


class Hunter final : public Character {
public:
	Hunter(
		std::vector<std::string> n_list,
		std::vector<std::string> g_list, 
		std::shared_ptr<std::unordered_map<std::string, int>> wc,
		bool isPlayer
	);
private:
	std::string className = "Hunter";
	std::vector<WeaponType> usableWeapon = {
		WeaponType::Bow
	}; 
};


class Pirate final : public Character {
public:
	Pirate(
		std::vector<std::string> n_list,
		std::vector<std::string> g_list, 
		std::shared_ptr<std::unordered_map<std::string, int>> wc,
		bool isPlayer
	);
private:
	std::string className = "Pirate";
	std::vector<WeaponType> usableWeapon = {
		WeaponType::Axe
	}; 
};


class CharacterManager{
public:
	CharacterManager(int level);
	void loadCharacter();
	void setInitialLevel(int level);
	//std::shared_ptr<Character> getCharacter(std::string id);
	//std::shared_ptr<Character> getPosCharacter(glm::vec2 a_pos);
	std::vector<std::shared_ptr<CameraGameObject>> getChildren();
private:
	std::unordered_map<std::string, std::shared_ptr<std::unordered_map<std::string, int>>> costTable = {};
	std::vector<std::shared_ptr<Character>> characters = {};
};

#endif