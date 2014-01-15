#pragma once
#include "Unit.h"
class unitManager
{
public:
	unitManager(void);
	~unitManager(void);
private:
	std::vector<Unit> * playerUnitList;
	std::vector<Unit> * enemyUnitList;
	Tile gameLevel[GAMEARRAYSIZE_MOCKUP][GAMEARRAYSIZE_MOCKUP]
};

