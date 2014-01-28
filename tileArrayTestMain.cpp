#include "StdAfx.h"
#include "Tile.h"
#include "SFML\Graphics.hpp"

bool foo(Tile theLevel[][128]){
	Tile  * level[] = theLevel;
	if(theLevel[3][3].open){
		printf("swag");
	}
}
int main(void){
	Tile gameLevel[128][128];
	foo(gameLevel);
	return 0;
}
