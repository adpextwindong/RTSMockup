#pragma once
#include <SFML\System\Vector2.hpp>

class Point2D
{
public:
	double x;
	double y;
	Point2D(sf::Vector2i);
	Point2D(double,double);
	void add(double x,double y);
	Point2D();
	~Point2D(void);
};

