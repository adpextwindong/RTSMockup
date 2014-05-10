#pragma once
#include "HealthBar.h"
#include <SFML\Graphics.hpp>
class controlPoint
{
public:
	int HPmax;
	int HPcurrent;
	HealthBar cpHealthBar;
	sf::Sprite cpSprite;
	controlPoint(sf::Texture * spriteTexture,sf::Vector2f position);
	~controlPoint(void);
};