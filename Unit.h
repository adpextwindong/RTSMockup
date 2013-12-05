#pragma once
#include "Point2D.h"
#include <SFML/Graphics.hpp>
#include "Command.h"
class Unit
{
	public:
		double speed;
		double HP;
		double size;
		bool selected;
		Point2D posistion;
		sf::Color Color;
		sf::CircleShape UnitShape;
		std::vector<Command> unitCommands;//queue for Players Commands
		std::vector<std::vector<Unit>*> listsThisUnitIsIn;//List of all the lists this unit is in
			//Used pointers to this Unit on death are removed

		Unit::Unit(Point2D _Posistion,sf::Color color,double size);
		~Unit(void);
		void Move(Point2D move2DVector);//Moves Unit
};

