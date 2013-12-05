#include "StdAfx.h"
#include "Unit.h"
#include <SFML/Graphics.hpp>

Unit::Unit(Point2D _Posistion,sf::Color _color,double _size){
	position=Point2D(_Posistion.x+_size,_Posistion.y+_size);
		//Makes circle center the Posistion
	Color=_color;
	size=_size;
	selected = false;
	UnitShape= sf::CircleShape(size);
	UnitShape.setPosition(position.x,position.y);
	UnitShape.setFillColor(Color);
	speed = 0.f;
	HP =0.f;
}

void Unit::Move(Point2D move2DVector){
	position.add(move2DVector.x,move2DVector.y);
}
Unit::~Unit(void)
{
}
