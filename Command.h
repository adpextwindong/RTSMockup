#pragma once
#include "Point2D.h"

enum CommandEnum {Attack, Move, Halt, Stop};

class Command
{
public:
	class Unit* target;
	CommandEnum theCommand;
	Point2D mousePosistion;
	
	Command(CommandEnum,Point2D,Unit *);
	~Command(void);
};

