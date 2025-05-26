#ifndef ITEM_HPP
#define ITEM_HPP

#include "Integration.hpp"
#include "Character/Character.hpp"

class Character;

class Item {
public:
	Item(std::string name, int uses, int worth, std::string note);
	virtual bool use(Character* user, std::shared_ptr<Character> target) { return false; }

	std::string getName() { return name; }
	int getUses() { return uses; }
	int getWorth() { return worth; }
	std::string getNote() { return note; }

	virtual std::shared_ptr<Item> clone() const { return std::make_shared<Item>(*this); }

protected:
	std::string name = "";
	int uses = 0;
	int worth = 0;
	std::string note = "";
};

class Vulnerary : public Item {
public:
	Vulnerary(std::vector<std::string> i_list);
	bool use(Character* user, std::shared_ptr<Character> target = nullptr) override;
	std::shared_ptr<Item> clone() const override { return std::make_shared<Vulnerary>(*this); }
};

class HandHeldItem : public Item {
public:
	HandHeldItem(std::vector<std::string> w_list);
	
	std::string getClassName() { return className; }
	int getMt() { return mt; }
	int getWt() { return wt; }
	int getHit() { return hit; }
	std::vector<int> getRng() { return rng; }
	int getWlv() { return wlv; }
	int getCrt() { return crt; }

protected:
	std::vector<std::string> against = {};
	std::string className = "";
	int mt = 0;
	int wt = 0;
	int hit = 0;
	std::vector<int> rng = {};
	int wlv = 0;
	int crt = 0;
};

class Weapon : public HandHeldItem{
public:
	Weapon(std::vector<std::string> w_list);
	bool use(Character* user, std::shared_ptr<Character> target) override;
	bool isAgainst(std::string className);
	std::shared_ptr<Item> clone() const override { return std::make_shared<Weapon>(*this); }
};

class Heal : public HandHeldItem {
public:
	Heal(std::vector<std::string> w_list);
	bool use(Character* user, std::shared_ptr<Character> target) override;
	std::shared_ptr<Item> clone() const override { return std::make_shared<Heal>(*this); }
};

#endif
