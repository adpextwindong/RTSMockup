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
	void spawnUnit(unsigned int num);
	void draw(sf::RenderWindow * win);
	unitSpawner::unitSpawner(std::vector<Unit> * _teamUnitList,Unit _unitTemplate,sf::Vector2i _position,sf::Texture _spawnerText);
	~unitSpawner(void);
};

