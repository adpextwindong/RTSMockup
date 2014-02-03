#include "StdAfx.h"
#include "unitManager.h"
#include "Macros.h"

unitManager::unitManager(std::vector<Unit> * thePlayerUnitList,std::vector<Unit> * theEnemyUnitList,sf::Vector2f * theViewPosition,Tile (&theGameLevel)[GAMEARRAYSIZE_MOCKUP][GAMEARRAYSIZE_MOCKUP] ){
	playerUnitList=thePlayerUnitList;
	enemyUnitList=theEnemyUnitList;
	//TODO FIX AND TEST THIS
	gameLevel=theGameLevel;
	viewPosition=theViewPosition;//used for float approximation
}
void unitManager::addUnit(bool playerUnit,int x,int y,double size){//int for tile location relative to window
	if(playerUnit){
		playerUnitList->push_back(Unit(sf::Vector2i(x,y),PLAYER_COLOR,size));
	}else{
		enemyUnitList->push_back(Unit(sf::Vector2i(x,y),ENEMY_COLOR,size));
	}
}
void unitManager::addUnit(bool playerUnit,float approxX,float approxY,double size){//float for approximate graphical location
	sf::Vector2i tileCord=sf::Vector2i((int)((*viewPosition).x+approxX)/TILE_SIZE , (int) ((*viewPosition).y+approxY)/TILE_SIZE);
	//TODO ADD ALGORITHM TO FIND OPEN SPOT
	//TODO Implement direction to start in. EX: Building calls to add unit, needs to start algorithm in that direction.
	if(gameLevel[tileCord.x][tileCord.y].open){

	}
	if(playerUnit){
		playerUnitList->push_back(Unit(tileCord,PLAYER_COLOR,size));
	}else{
		enemyUnitList->push_back(Unit(tileCord,ENEMY_COLOR,size));
	}
}
//