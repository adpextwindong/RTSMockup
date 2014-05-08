#pragma once
#include <SFML/Graphics.hpp>

class HealthBar
{
public:
	HealthBar(int *, int *, sf::CircleShape *);
	HealthBar(void);
	~HealthBar(void);

	sf::CircleShape * pShape;

	int *HPmax;
	int *HPcurrent;

	sf::RectangleShape HPgreen;
	sf::RectangleShape HPred;
	void HealthBar::HPupdate(sf::Vector2f position,int *);

};