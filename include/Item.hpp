// #ifndef ITEM_HPP
// #define ITEM_HPP

// #include "Integration.hpp"

// class Item {
// public:
// 	Item(int uses);
// 	virtual use(std::shared_ptr<Character> target) = 0;

// 	std::string getName() { return name; }
// 	int getUses() { return uses; }
// 	int getWorth() { return worth;}
// 	std::string getNote() { return note; }

// private:
// 	std::string name = "";
// 	int uses = 0;
// 	int worth = 0;
// 	std::string notes = "";

// };

// class Weapon : public Item{
// public:
// 	Weapon(std::vector<std::string> w_list);
// 	void use() override;

// 	int getWt() { return wt; }
// 	int getHit() { return hit; }
// 	int getRng() { return rng; }
// 	int getWlv() { return wlv; }
// 	int getCrt() { return crt; }

// private:
// 	int wt = 0;
// 	int hit = 0;
// 	int rng = 0;
// 	int	wlv = 0;
// 	int crt	= 0;
// };

// class Rapier : public Weapon{
// public:
// 	Rapier(std::vector<std::string> w_list);
// 	void use() override;
// }

// class IronSword : public Weapon{
// public:
// 	IronSword(std::vector<std::string> w_list);
// }

// class Lance : public Weapon{
// public:
// 	Lance(std::vector<std::string> w_list);
// }

// class SilverLance : public Weapon{
// public:
// 	SilverLance(std::vector<std::string> w_list);
// }

// class Javelin : public Weapon{
// public:
// 	Javelin(std::vector<std::string> w_list);
// }

// class Bow : public Weapon{
// public:
// 	Bow(std::vector<std::string> w_list);
// }

// class SteelBow : public Weapon{
// public:
// 	SteelBow(std::vector<std::string> w_list);
// }

// class Bowgun : public Weapon{
// public:
// 	Bowgun(std::vector<std::string> w_list);
// }

// class Heal : public Weapon{
// public:
// 	Heal(std::vector<std::string> w_list);
// 	void use() override;
// }

// class Axe : public Weapon{
// public:
// 	Axe(std::vector<std::string> w_list);
// }

// class SteelAxe : public Weapon{
// public:
// 	SteelAxe(std::vector<std::string> w_list);
// }

// class HandAxe : public Weapon{
// public:
// 	HandAxe(std::vector<std::string> w_list);
// }

// class Hammer : public Weapon{
// public:
// 	Hammer(std::vector<std::string> w_list);
// 	void use() override;
// }

// #endif