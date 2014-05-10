#pragma once
#include "Unit.h"
#include <SFML\Graphics.hpp>
class unitSpawner
{
public:
	sf::Sprite spawnerSprite;
	const Unit unitTemplate;
	sf::Vector2f position;
	std::vector<Unit> * teamUnitList;
	void unitSpawner::spawnUnit(unsigned int num,unsigned spreadRange);
	void draw(sf::RenderWindow * win);
	unitSpawner::unitSpawner(std::vector<Unit> * _teamUnitList,Unit _unitTemplate,sf::Vector2i _position,sf::Texture * _spawnerText);
	~unitSpawner(void);
};

int giveRand(unsigned int absRange);