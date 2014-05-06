#include "StdAfx.h"
#include "HealthBar.h"
#include <SFML/Graphics.hpp>
#include "iostream"

HealthBar::HealthBar(int *pointer1, int *pointer2, sf::CircleShape * pShape1)
{
	HPmax = pointer1;
	HPcurrent = pointer2;
	pShape = pShape1;
	printf("HPcurrent = %d",*HPcurrent);
	HPred.setSize(sf::Vector2f(50,10));
	HPgreen.setSize(sf::Vector2f(50,10));

	HPred.setFillColor(sf::Color::Red);
	HPgreen.setFillColor(sf::Color::Green);
	HPred.setFillColor(sf::Color::Red);
}
HealthBar::HealthBar(void){

}

void HealthBar::HPupdate(sf::Vector2f position, int * HPpointer) {
	//std::cout << position.x << "," << position.y << std::endl;
	HPcurrent = HPpointer;
	sf::Vector2f size = sf::Vector2f(((*HPmax - *HPcurrent) / (*HPmax/50)), 10);
	HPred.setSize(size);
	HPgreen.setPosition(position.x - 15, position.y - 30);
	HPred.setPosition(position.x - 15, position.y - 30);
	std::cout << *HPcurrent << std::endl;
	//std::cout << HPgreen.getPosition().x << "," << HPgreen.getPosition().y << std::endl;
	
}

HealthBar::~HealthBar(void)
{
}
