#include "stdafx.h"
#include "tile.h"
#include "macros.h"

int main(void){
	std::srand(time(NULL));
	printf("{");
	for(unsigned int i=0;i<10;i++){
		for(unsigned int j=0;j<10;j++){
			printf("%d,",rand()%2);
		}
		printf("\n");
	}
	printf("}");
	unsigned int myArray[10][10]=  {1,0,0,0,0,1,1,1,1,0,
									0,0,1,0,1,1,1,0,1,0,
									1,0,0,1,1,1,0,0,0,1,
									0,0,0,1,0,0,0,0,1,1,
									0,1,1,1,0,1,0,1,1,0,
									1,0,0,0,1,1,1,1,1,1,
									1,0,1,1,1,0,1,1,0,0,
									0,0,1,0,1,1,1,0,1,0,
									0,0,0,0,0,1,1,1,0,0,
									1,1,0,0,0,1,0,1,0,0,
									};
	Tile gameLevel[10][10];
	for(unsigned int i=0;i<10;i++){
		for(unsigned int j=0;j<10;j++){
			gameLevel[i][j]=Tile(myArray[i][j]);
		}
	}
	printf("Done");

	return 0;
}