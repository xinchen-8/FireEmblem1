#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include <queue>
#include "Integration.hpp"
#include "Tile.hpp"

struct DijkstraNode {
    int cost;
    glm::ivec2 pos;
    std::queue<glm::ivec2> path;

    bool operator>(const DijkstraNode& other) const {
        return cost > other.cost;
    }
};

class Character : public CameraGameObject {
public:
	Character(
		std::shared_ptr<MapManager> mm,
		std::vector<std::string> n_list,
		std::vector<std::string> g_list,
		std::shared_ptr<std::unordered_map<std::string, int>> wc,
		bool isPlayer
	);	
	virtual std::shared_ptr<Character> clone() = 0;

	void walkDirectly();
	void buildWalkPath(glm::ivec2 a_pos); //must in moveRange
	void refreshMoveRange(std::unordered_set<glm::ivec2> mask);
	void clearWalkPath();

	void setAnimation();
	void setStatus(CharacterStatus status);
	void setForword(Forword forword){ this->forword = forword; }

	void setVisible(bool visible){ m_Visible = visible; }
	void setHeadshotAnimation(std::shared_ptr<Util::Animation> a_headshotAnimation){ headshotAnimation = a_headshotAnimation; }
	void setTileAnimation();
	
	std::string getName() const { return name; }
	std::string getClassName() const { return className; }
	int getLevel() const { return Lv; }
	int getExp() const { return Ex; }
	int getHpLimit() const { return Hp_Limit; }
	int getCurHP() const { return Hp_Current; }
	int getStr() const { return Str; }
	int getSkl() const { return Skl; }
	int getWlv() const { return Wlv; }
	int getSpd() const { return Spd; }
	int getLck() const { return Lck; }
	int getDef() const { return Def; }
	int getRes() const { return Res; }
	int getMov() const { return Mov; }
	int getHPGR() const { return HPGR; }
	int getStrGR() const { return StrGR; }
	int getSklGR() const { return SklGR; }
	int getWlvGR() const { return WlvGR; }
	int getSpdGR() const { return SpdGR; }
	int getLckGR() const { return LckGR; }
	int getDefGR() const { return DefGR; }
	int getResGR() const { return ResGR; }
	std::unordered_map<glm::ivec2, int> getMoveRange() { return moveRange; }

	//items
protected:
	int gapOfAnimation = 3;
	//mask: enemy pos and locked tiles of map
	void findMoveRange(int mov, glm::ivec2 a_pos, std::unordered_set<glm::ivec2> mask);
private:
	std::string name = "";
	std::string className = "";
	bool isPlayer = true;

	Forword forword = Forword::Down;
	CharacterStatus status = CharacterStatus::Normal;
	std::queue<glm::ivec2> walkPath = {};
	std::vector<WeaponType> usableWeapon = {}; 

	int Lv = 0; int Ex = 0;
	int Hp_Limit = 0; int Hp_Current = 0;
	int Str = 0; int Skl = 0;
	int Wlv = 0; int Spd = 0;
	int Lck = 0; int Def = 0;
	int Res = 0; int Mov = 0;
	
	int HPGR = 0; int StrGR = 0;
	int SklGR = 0; int WlvGR = 0;
	int SpdGR = 0; int LckGR = 0;
	int DefGR = 0; int ResGR = 0;

	std::shared_ptr<MapManager> mapManager = nullptr;

	std::shared_ptr<Util::Animation> headshotAnimation = nullptr;
	std::shared_ptr<Util::Animation> standAnimation = nullptr;
	std::unordered_map<Forword, std::shared_ptr<Util::Animation>> walkAnimation = {};
	std::shared_ptr<Util::Animation> waitAnimation = nullptr;

	std::shared_ptr<std::unordered_map<std::string, int>> walkCost = nullptr;
	std::unordered_map<glm::ivec2, int> moveRange = {};
};

//class of characters
class Lord final : public Character {
public:
	Lord(std::shared_ptr<MapManager> mm,
			std::vector<std::string> n_list,
			std::vector<std::string> g_list, 
			std::shared_ptr<std::unordered_map<std::string, int>> wc,
			bool isPlayer);

	std::shared_ptr<Character> clone() override {
		auto c = std::make_shared<Lord>(*this);
		if (m_Drawable) {
			c->SetDrawable(std::make_shared<Util::Animation>(*std::dynamic_pointer_cast<Util::Animation>(m_Drawable)));
		}
		return c;
	}

private:
	std::string className = "Lord";
	std::vector<WeaponType> usableWeapon = { WeaponType::Sword }; 
};

class PegasusKnight final : public Character {
public:
	PegasusKnight(std::shared_ptr<MapManager> mm,
					std::vector<std::string> n_list,
					std::vector<std::string> g_list, 
					std::shared_ptr<std::unordered_map<std::string, int>> wc,
					bool isPlayer);

	std::shared_ptr<Character> clone() override {
		auto c = std::make_shared<PegasusKnight>(*this);
		if (m_Drawable) {
			c->SetDrawable(std::make_shared<Util::Animation>(*std::dynamic_pointer_cast<Util::Animation>(m_Drawable)));
		}
		return c;
	}
private:
	std::string className = "PegasusKnight";
	std::vector<WeaponType> usableWeapon = { WeaponType::Sword, WeaponType::Lance }; 
};

class Paladin final : public Character {
public:
	Paladin(std::shared_ptr<MapManager> mm,
			std::vector<std::string> n_list,
			std::vector<std::string> g_list, 
			std::shared_ptr<std::unordered_map<std::string, int>> wc,
			bool isPlayer);

	std::shared_ptr<Character> clone() override {
		auto c = std::make_shared<Paladin>(*this);
		if (m_Drawable) {
			c->SetDrawable(std::make_shared<Util::Animation>(*std::dynamic_pointer_cast<Util::Animation>(m_Drawable)));
		}
		return c;
	}

private:
	std::string className = "Paladin";
	std::vector<WeaponType> usableWeapon = { WeaponType::Sword, WeaponType::Lance }; 
};

class Cavalier final : public Character {
public:
	Cavalier(std::shared_ptr<MapManager> mm,
				std::vector<std::string> n_list,
				std::vector<std::string> g_list, 
				std::shared_ptr<std::unordered_map<std::string, int>> wc,
				bool isPlayer);

	std::shared_ptr<Character> clone() override {
		auto c = std::make_shared<Cavalier>(*this);
		if (m_Drawable) {
			c->SetDrawable(std::make_shared<Util::Animation>(*std::dynamic_pointer_cast<Util::Animation>(m_Drawable)));
		}
		return c;
	}

private:
	std::string className = "Cavalier";
	std::vector<WeaponType> usableWeapon = { WeaponType::Sword, WeaponType::Lance }; 
};

class Knight final : public Character {
public:
	Knight(std::shared_ptr<MapManager> mm,
			std::vector<std::string> n_list,
			std::vector<std::string> g_list, 
			std::shared_ptr<std::unordered_map<std::string, int>> wc,
			bool isPlayer);

	std::shared_ptr<Character> clone() override {
		auto c = std::make_shared<Knight>(*this);
		if (m_Drawable) {
			c->SetDrawable(std::make_shared<Util::Animation>(*std::dynamic_pointer_cast<Util::Animation>(m_Drawable)));
		}
		return c;
	}

private:
	std::string className = "Knight";
	std::vector<WeaponType> usableWeapon = { WeaponType::Sword, WeaponType::Lance }; 
};

class Thief final : public Character {
public:
	Thief(std::shared_ptr<MapManager> mm,
			std::vector<std::string> n_list,
			std::vector<std::string> g_list, 
			std::shared_ptr<std::unordered_map<std::string, int>> wc,
			bool isPlayer);

	std::shared_ptr<Character> clone() override {
		auto c = std::make_shared<Thief>(*this);
		if (m_Drawable) {
			c->SetDrawable(std::make_shared<Util::Animation>(*std::dynamic_pointer_cast<Util::Animation>(m_Drawable)));
		}
		return c;
	}

private:
	std::string className = "Thief";
	std::vector<WeaponType> usableWeapon = { WeaponType::Sword }; 
};

class Archer final : public Character {
public:
	Archer(std::shared_ptr<MapManager> mm,
			std::vector<std::string> n_list,
			std::vector<std::string> g_list, 
			std::shared_ptr<std::unordered_map<std::string, int>> wc,
			bool isPlayer);

	std::shared_ptr<Character> clone() override {
		auto c = std::make_shared<Archer>(*this);
		if (m_Drawable) {
			c->SetDrawable(std::make_shared<Util::Animation>(*std::dynamic_pointer_cast<Util::Animation>(m_Drawable)));
		}
		return c;
	}

private:
	std::string className = "Archer";
	std::vector<WeaponType> usableWeapon = { WeaponType::Bow }; 
};

class Curate final : public Character {
public:
	Curate(std::shared_ptr<MapManager> mm,
			std::vector<std::string> n_list,
			std::vector<std::string> g_list, 
			std::shared_ptr<std::unordered_map<std::string, int>> wc,
			bool isPlayer);

	std::shared_ptr<Character> clone() override {
		auto c = std::make_shared<Curate>(*this);
		if (m_Drawable) {
			c->SetDrawable(std::make_shared<Util::Animation>(*std::dynamic_pointer_cast<Util::Animation>(m_Drawable)));
		}
		return c;
	}

private:
	std::string className = "Curate";
	std::vector<WeaponType> usableWeapon = { WeaponType::Staff }; 
};

class Mercenary final : public Character {
public:
	Mercenary(std::shared_ptr<MapManager> mm,
				std::vector<std::string> n_list,
				std::vector<std::string> g_list, 
				std::shared_ptr<std::unordered_map<std::string, int>> wc,
				bool isPlayer);

	std::shared_ptr<Character> clone() override {
		auto c = std::make_shared<Mercenary>(*this);
		if (m_Drawable) {
			c->SetDrawable(std::make_shared<Util::Animation>(*std::dynamic_pointer_cast<Util::Animation>(m_Drawable)));
		}
		return c;
	}

private:
	std::string className = "Mercenary";
	std::vector<WeaponType> usableWeapon = { WeaponType::Sword }; 
};

class Fighter final : public Character {
public:
	Fighter(std::shared_ptr<MapManager> mm,
			std::vector<std::string> n_list,
			std::vector<std::string> g_list, 
			std::shared_ptr<std::unordered_map<std::string, int>> wc,
			bool isPlayer);

	std::shared_ptr<Character> clone() override {
		auto c = std::make_shared<Fighter>(*this);
		if (m_Drawable) {
			c->SetDrawable(std::make_shared<Util::Animation>(*std::dynamic_pointer_cast<Util::Animation>(m_Drawable)));
		}
		return c;
	}

private:
	std::string className = "Fighter";
	std::vector<WeaponType> usableWeapon = { WeaponType::Axe }; 
};

class Hunter final : public Character {
public:
	Hunter(std::shared_ptr<MapManager> mm,
			std::vector<std::string> n_list,
			std::vector<std::string> g_list, 
			std::shared_ptr<std::unordered_map<std::string, int>> wc,
			bool isPlayer);

	std::shared_ptr<Character> clone() override {
		auto c = std::make_shared<Hunter>(*this);
		if (m_Drawable) {
			c->SetDrawable(std::make_shared<Util::Animation>(*std::dynamic_pointer_cast<Util::Animation>(m_Drawable)));
		}
		return c;
	}

private:
	std::string className = "Hunter";
	std::vector<WeaponType> usableWeapon = { WeaponType::Bow }; 
};

class Pirate final : public Character {
public:
	Pirate(std::shared_ptr<MapManager> mm,
			std::vector<std::string> n_list,
			std::vector<std::string> g_list, 
			std::shared_ptr<std::unordered_map<std::string, int>> wc,
			bool isPlayer);

	std::shared_ptr<Character> clone() override {

		std::shared_ptr<Character> c = std::make_shared<Pirate>(*this);
		if (m_Drawable) {
			c->SetDrawable(std::make_shared<Util::Animation>(*std::dynamic_pointer_cast<Util::Animation>(m_Drawable)));
		}
		return c;
	}

private:
	std::string className = "Pirate";
	std::vector<WeaponType> usableWeapon = { WeaponType::Axe }; 
};

#endif