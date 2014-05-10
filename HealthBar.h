#pragma once
#include <SFML/Graphics.hpp>

class HealthBar
{
public:
	HealthBar::HealthBar(int *_HPmax, int *_HPcurrent);
	HealthBar(void);
	~HealthBar(void);

	int *HPmax;
	int *HPcurrent;

	sf::RectangleShape HPgreen;
	sf::RectangleShape HPred;
	void HealthBar::HPupdate(sf::Vector2f position,int *);

};