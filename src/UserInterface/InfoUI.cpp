#include "UserInterface/InfoUI.hpp"

TileInfoUI::TileInfoUI(std::vector<std::shared_ptr<Tile>>& tiles) :
	UserInterface(tiles){
	setUISize({ 4, 2 });

	setRelativePos({
		- floor(PTSD_Config::WINDOW_WIDTH / 2) + 1 * TILE_SIZE,
		- floor(PTSD_Config::WINDOW_HEIGHT / 2) + 1 * TILE_SIZE
	});
}

void TileInfoUI::load(std::shared_ptr<Tile> tile) {
	if(tile) this->tile = tile;
}

void TileInfoUI::update(){
	setString(tile->getName()+"\navoid +"+std::to_string(tile->getAvoid()));
	// setAbsolutePos({ TILE_SIZE, TILE_SIZE });
}

CharacterInfoUI::CharacterInfoUI(std::vector<std::shared_ptr<Tile>>& tiles) :
	UserInterface(tiles) {
	setUISize({ 5, 3 });

	setRelativePos({
		+ floor(PTSD_Config::WINDOW_WIDTH / 2) - 5 * TILE_SIZE,
		- floor(PTSD_Config::WINDOW_HEIGHT / 2) + 1 * TILE_SIZE
	});
}

void CharacterInfoUI::load(std::shared_ptr<Character> character) {
	if (character) this->character = character;
}

void CharacterInfoUI::update() {
    std::string content=
		character->getName() + "\n" +
		character->getClassName() + "\n" +
		"HP " + std::to_string(character->getCurHP()) + "/" + std::to_string(character->getHpLimit()) + "\n"; //first getHP() will be replaced by getCurHP()
    setString(content);
}