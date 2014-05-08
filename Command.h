#pragma once
#include "Point2D.h"
#include "SFML\Graphics.hpp"
enum CommandEnum {Attack, Move, Halt, Stop};

class Command
{
public:
	class Unit* target;
	CommandEnum theCommand;
	sf::Vector2i mousePosition;
	
	Command(CommandEnum,sf::Vector2i,Unit *);
	~Command(void);
};

