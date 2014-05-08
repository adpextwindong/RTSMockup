#pragma once
#include "Unit.h"
#include "Tile.h"
#include "Macros.h"

class unitManager//MUH SINGLETON GOD CLASS
{
public:
	void addUnit(bool playerUnit,int x,int y,double size);
	void unitManager::addUnit(bool playerUnit,float approxX,float approxY,double size);
	unitManager(std::vector<Unit> * testList);
	unitManager(std::vector<Unit> * thePlayerUnitList,std::vector<Unit> * theEnemyUnitList,sf::Vector2f * theViewPosition,Tile (&theGameLevel)[GAMEARRAYSIZE_MOCKUP][GAMEARRAYSIZE_MOCKUP]);
	//unitManager::unitManager(std::vector<Unit> * testList,std::vector<Unit> * testList2);
private:
	std::vector<Unit> * playerUnitList;
	std::vector<Unit> * enemyUnitList;
	Tile (*gameLevel)[GAMEARRAYSIZE_MOCKUP];
	sf::Vector2f * viewPosition; 
};