#pragma once
#include "Point2D.h"
#include <SFML/Graphics.hpp>
#include "Command.h"
#include "HealthBar.h"

class Unit
{
	public:
		double speed;
		double HP;
		double size;
		bool selected;
		Point2D position;
		int HPmax;
		int HPcurrent;
		HealthBar unitHealthBar;
		sf::Color Color;
		sf::CircleShape UnitShape;// unit sprite
		std::vector<Command> unitCommands;//queue for Players Commands
		std::vector<std::vector<Unit>*> listsThisUnitIsIn;//List of all the lists this unit is in
			//Used pointers to this Unit on death are removed

		Unit::Unit(Point2D _Posistion,sf::Color color,double size);
		~Unit(void);
		void Move(Point2D move2DVector);//Moves Unit
};

