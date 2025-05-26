#ifndef INFOUI_HPP
#define INFOUI_HPP
#include "UserInterface.hpp"

class TileInfoUI : public UserInterface {
public:
	TileInfoUI(std::vector<std::shared_ptr<Tile>>& tiles);
	void load(std::shared_ptr<Tile> tile);
	void update() override;
private:
	std::shared_ptr<Tile> tile = nullptr;
};

class CharacterInfoUI : public UserInterface {
public:
	CharacterInfoUI(std::vector<std::shared_ptr<Tile>>& tiles);
	void load(std::shared_ptr<Character> character);
	void update() override;
private:
	std::shared_ptr<Character> character = nullptr;
};

#endif