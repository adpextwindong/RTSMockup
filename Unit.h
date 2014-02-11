#pragma once
#include "Point2D.h"
#include <SFML/Graphics.hpp>
#include "Command.h"
#include "Macros.h"
class Unit
{
	public:
		unsigned char moveTickAmount;
		unsigned short HPmax;
		unsigned short HPcurrent;
		unsigned char size;//Pixel Ammount
		sf::Vector2i posistion;//Posistion within gameLevel
		Team playerTeam;
		sf::Color unitColor;
		sf::CircleShape UnitShape;
		
		std::vector<Command> unitCommands;//queue for Players Commands
		
		Unit::Unit(sf::Vector2i posistion,sf::Color color,double size);
		~Unit(void);
		bool Unit::Move(enum adjacentDirection direction);//Moves Unit
};

