#include "StdAfx.h"
#include "Unit.h"
#include <SFML/Graphics.hpp>

Unit::Unit(sf::Vector2i _posistion,sf::Color _color,double _size){
	posistion=_posistion;
		//Makes circle center the Posistion
	Color=_color;
	size=_size;
	UnitShape= sf::CircleShape(size);
	UnitShape.setPosition(posistion.x,posistion.y);
	UnitShape.setFillColor(Color);
	moveTickAmount = 0.f;
	HP =0.f;
}
Unit::~Unit(void)
{
}
