#include "StdAfx.h"
#include "Unit.h"
#include <SFML/Graphics.hpp>

Unit::Unit(sf::Vector2i _posistion,Team teamParam,double _size){
	posistion=_posistion;
		//Makes circle center the Posistion
	teamParam
	size=_size;
	UnitShape= sf::CircleShape(size);
	UnitShape.setPosition(posistion.x,posistion.y);
	UnitShape.setFillColor(returnColor(playerTeam));
	moveTickAmount = 0.f;
	HPcurrent =0.f;
}
Unit::~Unit(void)
{
}
