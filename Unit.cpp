#include "StdAfx.h"
#include "Unit.h"
#include <SFML/Graphics.hpp>

Unit::Unit(Point2D _Posistion,sf::Color _color,double _size){
	posistion=Point2D(_Posistion.x+_size,_Posistion.y+_size);
		//Makes circle center the Posistion
	Color=_color;
	size=_size;
	selected = false;
	UnitShape= sf::CircleShape(size);
	UnitShape.setPosition(posistion.x,posistion.y);
	UnitShape.setFillColor(Color);
}

void Unit::Move(Point2D move2DVector){
	posistion.add(move2DVector.x,move2DVector.y);
}
Unit::~Unit(void)
{
}
