#include "StdAfx.h"
#include "Tile.h"
#include "SFML\Graphics.hpp"

void foo(bool (&theLevel)[2][2]){
	bool (*myArray)[2] = theLevel;
	if(myArray[0][0]){
		printf("swag\n");
	}else{
		printf("not swag");
		myArray[0][0]=true;
	}
}
int main(void){
	bool gameLevel[2][2]={	false,false,
							false,false};
	foo(gameLevel);
	printf("\n%s\n",gameLevel[0][0]?"TRUE":"FALSE");
	foo(gameLevel);
	getchar();
	return 0;
}
