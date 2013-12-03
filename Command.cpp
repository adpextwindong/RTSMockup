#pragma once
#include "StdAfx.h"
#include "Command.h"
#include "Unit.h"

Command::Command(CommandEnum _theCommand,Point2D _mousePosistion,Unit * _target)
{
	CommandEnum theCommand=_theCommand;
	Point2D mousePosistion=_mousePosistion;
	Unit * target=_target;
}

Command::~Command(void)
{
}
