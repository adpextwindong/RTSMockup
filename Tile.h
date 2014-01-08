#pragma once
#include "Unit.h"
class Tile
{
	//tiles are 8px by 8px
	//see mockupTile.png
public:
	bool open;
	std::vector<Unit*> containingUnits;
	sf::Sprite tileSprite;
	Tile(void);
	Tile(bool);
	~Tile(void);
};

