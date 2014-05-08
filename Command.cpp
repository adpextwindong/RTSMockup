#pragma once
#include "StdAfx.h"
#include "Command.h"
#include "Unit.h"
#include "SFML\Graphics.hpp"
Command::Command(CommandEnum _theCommand,sf::Vector2i _mousePosition,Unit * _target):
	theCommand(_theCommand), mousePosition(_mousePosition), target(_target)
	{
	
	
	};

Command::~Command(void)
{
	
}
