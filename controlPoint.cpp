#include "StdAfx.h"
#include "controlPoint.h"
#include "Macros.h"

controlPoint::controlPoint(sf::Texture * spriteTexture,sf::Vector2f position)
{
	HPmax = ORGAN_HP_MAX;
	HPcurrent = ORGAN_HP_MAX;
	cpSprite.setPosition(position);
	cpHealthBar = HealthBar(&HPmax,&HPcurrent);
	cpSprite.setTexture(*spriteTexture);
}


controlPoint::~controlPoint(void)
{
}
