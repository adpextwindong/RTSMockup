#include "StdAfx.h"
#include "Point2D.h"
#include <SFML\System\Vector2.hpp>

Point2D::Point2D(sf::Vector2i theVector2f){
	x=theVector2f.x;
	y=theVector2f.y;
}
Point2D::Point2D(double _x,double _y)
{
	x=_x;
	y=_y;
}
void Point2D::add(double _x,double _y){
	x+=_x;
	y+=_y;
}
Point2D::Point2D(void){
	x=0;
	y=0;
}
Point2D::~Point2D(void)
{
}
