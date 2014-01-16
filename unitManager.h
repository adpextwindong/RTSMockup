#pragma once
#include "Unit.h"
#include "Tile.h"
#include "Macros.h"

class unitManager
{
public:
	void addUnit(bool playerUnit,int x,int y,double size);
	unitManager(std::vector<Unit> * testList);
	unitManager::unitManager(std::vector<Unit> * thePlayerUnitList,std::vector<Unit> * theEnemyUnitList,Tile * theGameLevel);
	//unitManager::unitManager(std::vector<Unit> * testList,std::vector<Unit> * testList2);
private:
	std::vector<Unit> * playerUnitList;
	std::vector<Unit> * enemyUnitList;
	Tile * gameLevel[GAMEARRAYSIZE_MOCKUP][GAMEARRAYSIZE_MOCKUP];
};

