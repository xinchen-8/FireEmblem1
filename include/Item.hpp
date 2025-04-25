#ifndef ITEM_HPP
#define ITEM_HPP

#include "Integration.hpp"
#include "Character.hpp"

class Character;

class Item {
public:
	Item(std::string name, int uses, int worth, std::string note);
	virtual void use(std::shared_ptr<Character> user, std::shared_ptr<Character> target) { uses--; }

	std::string getName() { return name; }
	int getUses() { return uses; }
	int getWorth() { return worth; }
	std::string getNote() { return note; }

	virtual std::shared_ptr<Item> clone() const = 0;

protected:
	std::string name = "";
	int uses = 0;
	int worth = 0;
	std::string note = "";
};

class Vulnerary : public Item {
public:
	Vulnerary(std::vector<std::string> i_list);
	void use(std::shared_ptr<Character> user, std::shared_ptr<Character> target) override;
	std::shared_ptr<Item> clone() const override { return std::make_shared<Vulnerary>(*this); }
};

class HandHeldItem : public Item {
public:
	HandHeldItem(std::vector<std::string> w_list);
	void use(std::shared_ptr<Character> user, std::shared_ptr<Character> target) override;

	bool isAgainst(std::string className);
	int getMt() { return mt; }
	int getWt() { return wt; }
	int getHit() { return hit; }
	std::vector<int> getRng() { return rng; }
	int getWlv() { return wlv; }
	int getCrt() { return crt; }

protected:
	std::vector<std::string> against = {};

private:
	int mt = 0;
	int wt = 0;
	int hit = 0;
	std::vector<int> rng = {};
	int wlv = 0;
	int crt = 0;
};

class Rapier : public HandHeldItem {
public:
	Rapier(std::vector<std::string> w_list);
	std::vector<std::string> against = {
		"Cavalier", "Knight", "Paladins", "Generals"
	};
	std::shared_ptr<Item> clone() const override { return std::make_shared<Rapier>(*this); }
};

class IronSword : public HandHeldItem {
public:
	IronSword(std::vector<std::string> w_list);
	std::shared_ptr<Item> clone() const override { return std::make_shared<IronSword>(*this); }
};

class SteelSword : public HandHeldItem {
public:
	SteelSword(std::vector<std::string> w_list);
	std::shared_ptr<Item> clone() const override { return std::make_shared<SteelSword>(*this); }
};

class IronLance : public HandHeldItem {
public:
	IronLance(std::vector<std::string> w_list);
	std::shared_ptr<Item> clone() const override { return std::make_shared<IronLance>(*this); }
};

class SilverLance : public HandHeldItem {
public:
	SilverLance(std::vector<std::string> w_list);
	std::shared_ptr<Item> clone() const override { return std::make_shared<SilverLance>(*this); }
};

class Javelin : public HandHeldItem {
public:
	Javelin(std::vector<std::string> w_list);
	std::shared_ptr<Item> clone() const override { return std::make_shared<Javelin>(*this); }
};

class IronBow : public HandHeldItem {
public:
	IronBow(std::vector<std::string> w_list);
	std::shared_ptr<Item> clone() const override { return std::make_shared<IronBow>(*this); }
};

class SteelBow : public HandHeldItem {
public:
	SteelBow(std::vector<std::string> w_list);
	std::shared_ptr<Item> clone() const override { return std::make_shared<SteelBow>(*this); }
};

class Bowgun : public HandHeldItem {
public:
	Bowgun(std::vector<std::string> w_list);
	std::shared_ptr<Item> clone() const override { return std::make_shared<Bowgun>(*this); }
};

class Heal : public HandHeldItem {
public:
	Heal(std::vector<std::string> w_list);
	void use(std::shared_ptr<Character> user, std::shared_ptr<Character> target) override;
	std::shared_ptr<Item> clone() const override { return std::make_shared<Heal>(*this); }
};

class IronAxe : public HandHeldItem {
public:
	IronAxe(std::vector<std::string> w_list);
	std::shared_ptr<Item> clone() const override { return std::make_shared<IronAxe>(*this); }
};

class SteelAxe : public HandHeldItem {
public:
	SteelAxe(std::vector<std::string> w_list);
	std::shared_ptr<Item> clone() const override { return std::make_shared<SteelAxe>(*this); }
};

class HandAxe : public HandHeldItem {
public:
	HandAxe(std::vector<std::string> w_list);
	std::shared_ptr<Item> clone() const override { return std::make_shared<HandAxe>(*this); }
};

class Hammer : public HandHeldItem {
public:
	Hammer(std::vector<std::string> w_list);
	std::shared_ptr<Item> clone() const override { return std::make_shared<Hammer>(*this); }
};

#endif
