#pragma once
#include "Point2D.h"
#include <SFML/Graphics.hpp>
#include "Command.h"
class Unit
{
	public:
		double moveTickAmount;
		double HP;
		double size;//Pixel Ammount
		sf::Vector2i posistion;//Posistion within gameLevel
		sf::Color Color;
		sf::CircleShape UnitShape;
		std::vector<Command> unitCommands;//queue for Players Commands
		std::vector<std::vector<Unit>*> listsThisUnitIsIn;//List of all the lists this unit is in
			//Used pointers to this Unit on death are removed

		Unit::Unit(sf::Vector2i posistion,sf::Color color,double size);
		~Unit(void);
		bool Unit::Move(enum adjacentDirection direction);//Moves Unit
};

