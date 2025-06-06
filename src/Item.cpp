#include "Item.hpp"

Item::Item(std::string name, int uses, int worth, std::string note)
    : name(name), uses(uses), worth(worth), note(note) {}

Vulnerary::Vulnerary(std::vector<std::string> i_list)
    : Item(i_list[ITEM_INDEX::NAME], std::stoi(i_list[ITEM_INDEX::USES]), std::stoi(i_list[ITEM_INDEX::WORTH]),
           i_list[ITEM_INDEX::NOTE]) {}

bool Vulnerary::use(Character *user, std::shared_ptr<Character> target) {
    user->setHP(user->getCurHP() + 10);
    uses--;
    LOG_INFO("Vulerary Used Successfully.");
    return true;
}

HandHeldItem::HandHeldItem(std::vector<std::string> w_list)
    : Item(w_list[HANDHELD_INDEX::NAME], std::stoi(w_list[HANDHELD_INDEX::USES]),
           std::stoi(w_list[HANDHELD_INDEX::WORTH]), w_list[HANDHELD_INDEX::NOTE]) {

    // std::cout<<name<<std::endl;
    className = w_list[HANDHELD_INDEX::CLASS];
    mt = std::stoi(w_list[HANDHELD_INDEX::MT]);
    wt = std::stoi(w_list[HANDHELD_INDEX::WT]);
    hit = std::stoi(w_list[HANDHELD_INDEX::HIT]);
    std::string data = w_list[HANDHELD_INDEX::RNG];

    size_t pos = data.find("~");
    if (pos != std::string::npos) {
        int first = std::stoi(data.substr(0, pos));
        int second = std::stoi(data.substr(pos + 1));
        rng.push_back(first);
        rng.push_back(second);
    } else {
        rng.push_back(std::stoi(data));
    }

    wlv = std::stoi(w_list[HANDHELD_INDEX::WLV]);
    crt = std::stoi(w_list[HANDHELD_INDEX::CRT]);
}

Weapon::Weapon(std::vector<std::string> w_list) : HandHeldItem(w_list) {
    if (name == "Rapier")
        against = {"Cavalier", "Knight", "Paladin", "General"};
    else if (name == "Hammer")
        against = {"Knight", "General"};
    else if (name == "Steel Bow" || name == "Iron Bow")
        against = {"PegasusKnight"};
}

bool Weapon::use(Character *user, std::shared_ptr<Character> target) {
    int power = user->getStr() + mt * (isAgainst(target->getClassName()) ? 3 : 1);
    int critical = (user->getSkl() + user->getLck()) / 2 + crt;
    int accuracy = hit + user->getSkl();
    if (target->attacked(power, critical, accuracy)) {
        uses--;
        return true;
    }
    return false;
}

bool Weapon::isAgainst(std::string className) {
    for (auto &a : against) {
        if (a == className)
            return true;
    }
    return false;
}

Heal::Heal(std::vector<std::string> w_list) : HandHeldItem(w_list) {}

bool Heal::use(Character *user, std::shared_ptr<Character> target) {
    int reg = target->getCurHP();
    uses--;
    target->setHP(reg + 10);
    LOG_INFO(target->getName() + " HP+10: " + std::to_string(reg) + " -> " + std::to_string(target->getCurHP()));
    return true;
}
