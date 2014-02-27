// RTSMockup.cpp : Defines the entry point for the console application.
#include "stdafx.h"//VS2010 Requirement
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Unit.h"//Unit Object Class
#include "Command.h"//Unit Commands
#include "Tile.h"//Game Array Tile Class
#include "Macros.h"//Macros TODO Make config system
#include "fpsCounter.h"//FPS Counter
#include "unitManager.h"//Unit Manager Singleton TODO Wrap singleton classes into a clientInstance class for the client
#include "DrawHandler.h"//Client Draw Handler Singleton

enum DecisionState {Selecting,Commanding};

std::vector<Unit> s_playerUnits;
std::vector<Unit> s_enemyUnits;
Tile s_gameLevel[GAMEARRAYSIZE_MOCKUP][GAMEARRAYSIZE_MOCKUP];//game level

sf::RectangleShape c_clientSelectionShape;
sf::Vector2i c_originalSelectPoint;//SELECT LEFT CLICK POINT
sf::RenderWindow c_window(sf::VideoMode(1024, 768), "RTS Mockup");
sf::Vector2f c_viewPosition;//Used for scrolling the screen around.

//Mouse Selection & Commanding
std::vector<Unit *> c_playerSelection;
CommandEnum c_currentCommand;
//Mouse Logic Variables
bool c_leftMouseClickedLastCycle;
DecisionState c_mouseCommandState;
bool c_selectionDrawState;

void drawUnitVector(sf::RenderWindow * c_window,std::vector<Unit>* list){//draws all the units in a unit list
	for(unsigned int i=0;i<list->size();i++){
		(*c_window).draw((*list)[i].UnitShape);
	}
	
}

void updateSelection(){
	sf::Vector2i LimitedMousePos = sf::Mouse::getPosition(c_window);
	if(LimitedMousePos.x<0 || LimitedMousePos.x > c_window.getSize().x){//if 
		if(LimitedMousePos.x<0){
			LimitedMousePos.x=0;
		}else{
			LimitedMousePos.x=c_window.getSize().x;
		}
	}
	if(LimitedMousePos.y<0 || LimitedMousePos.y > c_window.getSize().y){//if 
		if(LimitedMousePos.y<0){
			LimitedMousePos.y=0;
		}else{
			LimitedMousePos.y=c_window.getSize().y;
		}
	}
	int yDiff=(LimitedMousePos.y-c_originalSelectPoint.y);
	if((LimitedMousePos.x-c_originalSelectPoint.x)<=0){//Xdiff +x
		if(yDiff<=0){//+x +y Quad 1
			c_clientSelectionShape.setPosition(c_originalSelectPoint.x,LimitedMousePos.y);
			c_clientSelectionShape.setSize(sf::Vector2f(LimitedMousePos.x-c_originalSelectPoint.x,c_originalSelectPoint.y-LimitedMousePos.y));
		}else{//+x -y Quad 4
			c_clientSelectionShape.setSize(sf::Vector2f(LimitedMousePos.x-c_originalSelectPoint.x,LimitedMousePos.y-c_originalSelectPoint.y));
		}
	}else{
		if(yDiff<=0){// -x +y Quad 2
			c_clientSelectionShape.setPosition(sf::Vector2f(LimitedMousePos));
			c_clientSelectionShape.setSize(sf::Vector2f(c_originalSelectPoint.x-LimitedMousePos.x,c_originalSelectPoint.y-LimitedMousePos.y));
		}else{// -x -y Quad 3
			c_clientSelectionShape.setPosition(sf::Vector2f(LimitedMousePos.x,c_originalSelectPoint.y));
			c_clientSelectionShape.setSize(sf::Vector2f(c_originalSelectPoint.x-LimitedMousePos.x,LimitedMousePos.y-c_originalSelectPoint.y));
		}
	}

	//printf("Shape Pos:%f %f\n",c_clientSelectionShape.getPosition().x,c_clientSelectionShape.getPosition().y);
	for(int i=0;i<4;i++){
		//printf("P(%d): %.0f,%.0f ",i,c_clientSelectionShape.getPoint(i).x+c_clientSelectionShape.getPosition().x,c_clientSelectionShape.getPoint(i).y+c_clientSelectionShape.getPosition().y);
	}
	//printf("\n\n");

	//printf("Limited MosPos %d %d ",LimitedMousePos.x,LimitedMousePos.y);
	//printf("Real MosPos %d %d\n",sf::Mouse::getPosition(c_window).x,sf::Mouse::getPosition(c_window).y);
	/*printf("Original Point %d %d ",c_originalSelectPoint.x,c_originalSelectPoint.y);
	printf("Selection Cords %f.0 %f.0\n\n",c_clientSelectionShape.getPosition().x,c_clientSelectionShape.getPosition().y);*/
}
void grabOnScreenSelectedUnits(std::vector<Unit>* s_playerUnits,std::vector<Unit *>* c_playerSelection){
	bool shifted = false;
	unsigned int preExistingElemCount=0;
	if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)&&!sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)){
		printf("UnShifted %S Command \n","Selection");
		(*c_playerSelection).clear();
		//printf("\nList cleared\n");
	}else{
		printf("Shifted %S Command \n","Selection");
		shifted=true;
		preExistingElemCount=(*c_playerSelection).size();
	}
	float minX=c_clientSelectionShape.getPoint(0).x+c_clientSelectionShape.getPosition().x;
	float maxX=0.f;
	float minY=c_clientSelectionShape.getPoint(0).y+c_clientSelectionShape.getPosition().y;
	float maxY=0.f;
	for(int i=0;i<4;i++){
		minX = minX > c_clientSelectionShape.getPoint(i).x+c_clientSelectionShape.getPosition().x ? c_clientSelectionShape.getPoint(i).x+c_clientSelectionShape.getPosition().x : minX;
		maxX = maxX < c_clientSelectionShape.getPoint(i).x+c_clientSelectionShape.getPosition().x ? c_clientSelectionShape.getPoint(i).x+c_clientSelectionShape.getPosition().x : maxX;
		minY = minY > c_clientSelectionShape.getPoint(i).y+c_clientSelectionShape.getPosition().y ? c_clientSelectionShape.getPoint(i).y+c_clientSelectionShape.getPosition().y : minY;
		maxY = maxY < c_clientSelectionShape.getPoint(i).y+c_clientSelectionShape.getPosition().y ? c_clientSelectionShape.getPoint(i).y+c_clientSelectionShape.getPosition().y : maxY;
	}
	float radius=0.f;
	for(unsigned int unitIterator=0;unitIterator<(*s_playerUnits).size();unitIterator++){//Iterate through all units
		Unit * currentPointer=NULL;
		bool containsThePointer = false;
		if(shifted==true){//If user is adding, check if current unit is on the list already
			currentPointer =&(*s_playerUnits)[unitIterator];
			for(unsigned int j=0;j<(*c_playerSelection).size();j++){//iterate through unit selection vector
				if(currentPointer==(*c_playerSelection)[j]){
					containsThePointer=true;
					if(c_clientSelectionShape.getSize().x ==0 && c_clientSelectionShape.getSize().y ==0){
						if(sqrt(std::pow((*s_playerUnits)[unitIterator].UnitShape.getRadius()+(*s_playerUnits)[unitIterator].UnitShape.getPosition().x-c_clientSelectionShape.getPosition().x,2.0f)+std::pow((*s_playerUnits)[unitIterator].UnitShape.getRadius()+(*s_playerUnits)[unitIterator].UnitShape.getPosition().y-c_clientSelectionShape.getPosition().y,2.0f))<=(*s_playerUnits)[unitIterator].UnitShape.getRadius()){
							(*c_playerSelection).erase((*c_playerSelection).begin()+j);
							unitIterator=(*s_playerUnits).size()+1;
						}
					}
					break;
				}
			}
		}
		if(containsThePointer==false){//if this element isn't in the list
			if(c_clientSelectionShape.getSize().x ==0 && c_clientSelectionShape.getSize().y ==0){
				if(sqrt(std::pow((*s_playerUnits)[unitIterator].UnitShape.getRadius()+(*s_playerUnits)[unitIterator].UnitShape.getPosition().x-c_clientSelectionShape.getPosition().x,2.0f)+std::pow((*s_playerUnits)[unitIterator].UnitShape.getRadius()+(*s_playerUnits)[unitIterator].UnitShape.getPosition().y-c_clientSelectionShape.getPosition().y,2.0f))<=(*s_playerUnits)[unitIterator].UnitShape.getRadius()){
					(*c_playerSelection).push_back(&(*s_playerUnits)[unitIterator]);
				}
			}else{
				radius=(*s_playerUnits)[unitIterator].UnitShape.getRadius()/2;
				sf::Vector2f position((*s_playerUnits)[unitIterator].UnitShape.getPosition().x,(*s_playerUnits)[unitIterator].UnitShape.getPosition().y);
				if(position.x >= minX && position.x <= maxX){
					if(position.y >= minY && position.y <= maxY){
						(*c_playerSelection).push_back(&(*s_playerUnits)[unitIterator]);
					}
				}
			}
		}
	}
}
char * commandString(CommandEnum theCommand){
	switch(theCommand){
	case Attack:
		return "Attack";
	case Move:
		return "Move";
	case Halt:
		return "Halt";
	case Stop:
		return "Stop";
	}
}
Unit * unitAtMousePos(){//TODO: FINISH THIS FUNCTION
	//check for target unit at mouse posisiton
	for(unsigned int i=0;i<s_playerUnits.size();i++){
		float radius = s_playerUnits[i].UnitShape.getRadius();

		float unitPosx = s_playerUnits[i].UnitShape.getPosition().x + radius;
		float unitPosy = s_playerUnits[i].UnitShape.getPosition().y + radius;

		float mousPosx = sf::Mouse::getPosition(c_window).x;
		float mousPosy = sf::Mouse::getPosition(c_window).y;

		float distance = sqrt(pow((unitPosx-mousPosx),2)+pow((unitPosy-mousPosy),2));
		if(distance<=radius){
			printf("%d",&s_playerUnits[i]);
			return &s_playerUnits[i];
		}
	}

	for(unsigned int i=0;i<s_enemyUnits.size();i++){
		float radius = s_enemyUnits[i].UnitShape.getRadius();
		float unitPosx = s_enemyUnits[i].UnitShape.getPosition().x + radius;
		float unitPosy = s_enemyUnits[i].UnitShape.getPosition().y + radius;
		float mousPosx = sf::Mouse::getPosition(c_window).x;
		float mousPosy = sf::Mouse::getPosition(c_window).y;

		float distance = sqrt(pow((unitPosx-mousPosx),2)+pow((unitPosy-mousPosy),2));
		if(distance<=radius){
			return &s_enemyUnits[i];
		}
	}
	
	return NULL;
}
void CommandSelectionUnits(CommandEnum theCommand, std::vector<Unit *> * c_playerSelection){
	for(unsigned int i=0;i<(*c_playerSelection).size();i++){
		if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)&&!sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)){
			printf("UnShifted %S Command",commandString(theCommand));
			(*(*c_playerSelection)[i]).unitCommands.empty();
		}
		printf("Shifted %S Command",commandString(theCommand));
		(*(*c_playerSelection)[i]).unitCommands.push_back(Command(theCommand,Point2D(sf::Mouse::getPosition(c_window)),unitAtMousePos()));
	}
}


bool mouseIsOnScreen(){
	//printf("%d %d\n",sf::Mouse::getPosition(c_window).x,sf::Mouse::getPosition(c_window).y);
	if((sf::Mouse::getPosition().x-c_window.getPosition().x >0) && (sf::Mouse::getPosition(c_window).x <=c_window.getSize().x)){
		if((sf::Mouse::getPosition().y-c_window.getPosition().y >0) && (sf::Mouse::getPosition(c_window).y <=c_window.getSize().y)){
			//printf("\nOnscreen\n");
			return true;
		}
	}
	return false;
}
void drawSelectionStroke(std::vector<Unit*>* c_playerSelection){
	sf::CircleShape strokeShape(0.f);
	strokeShape.setFillColor(SELECTION_STROKE_COLOR);
	//printf(" %d\n",(*c_playerSelection).size());
	for(unsigned int i=0;i<(*c_playerSelection).size();i++){
		strokeShape.setRadius((*(*c_playerSelection)[i]).UnitShape.getRadius()+STROKE_SIZE);
		strokeShape.setPosition((*(*c_playerSelection)[i]).UnitShape.getPosition().x-STROKE_SIZE,(*(*c_playerSelection)[i]).UnitShape.getPosition().y-STROKE_SIZE);
		c_window.draw(strokeShape);
	}
}
void setQuadPos(sf::Vertex * quad,const unsigned int i,const unsigned int j){
	quad[0].position = sf::Vector2f(i * TILE_SIZE, j * TILE_SIZE);
    quad[1].position = sf::Vector2f((i + 1) * TILE_SIZE, j * TILE_SIZE);
    quad[2].position = sf::Vector2f((i + 1) * TILE_SIZE, (j + 1) * TILE_SIZE);
    quad[3].position = sf::Vector2f(i * TILE_SIZE, (j + 1) * TILE_SIZE);
}

void setQuadTexture(sf::Vertex * quad,const unsigned int i,const unsigned int j){//TODO MAKE TILESHEET IMPLEMENTATION
	printf("\n\n");
	//	FALSE		TRUE
	//0,0  32,0		32,0  64,0
	//0,32 32,32	32,32 64,32
	sf::Vector2f cordOrder[4]={ sf::Vector2f((s_gameLevel[i][j].open*TILE_SIZE)+0.0,0.0),
								sf::Vector2f((s_gameLevel[i][j].open*TILE_SIZE)+TILE_SIZE,0.0),
								sf::Vector2f((s_gameLevel[i][j].open*TILE_SIZE)+TILE_SIZE,TILE_SIZE),
								sf::Vector2f((s_gameLevel[i][j].open*TILE_SIZE)+0.0,TILE_SIZE)
	};
	quad[0].texCoords = cordOrder[0];//TOP LEFT
	quad[1].texCoords = cordOrder[1];//TOP RIGHT
	quad[2].texCoords = cordOrder[2];//BOTTOM RIGHT
	quad[3].texCoords = cordOrder[3];//BOTTOM LEFT
	//if(s_gameLevel[i][j].open){
		//printf("%s",s_gameLevel[i][j].open?"TRUE\n":"FALSE\n");
		//printf("TOP LEFT %f %f\n",quad[0].texCoords.x,quad[0].texCoords.y);
		//printf("TOP RIGHT %f %f\n",quad[1].texCoords.x,quad[1].texCoords.y);
		//printf("BOTTOM RIGHT %f %f\n",quad[2].texCoords.x,quad[2].texCoords.y);
		//printf("BOTTOM LEFT %f %f\n",quad[3].texCoords.x,quad[3].texCoords.y);
		//printf("\n");
	//}
}
//void drawLevel(){ DEPRECATED FOR VERTEX ARRAY
//	for(unsigned int i=0;i<GAMEARRAYSIZE_MOCKUP;i++){
//		for(unsigned int j=0;j<GAMEARRAYSIZE_MOCKUP;j++){
//			c_window.draw(s_gameLevel[i][j].tileSprite);
//		}
//	}
//}
void mouseLogic(){
		//IF MOUSE IS ON THE c_window
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left)^c_leftMouseClickedLastCycle){//if Left Mouse Button state has changed since last cycle
			if(c_leftMouseClickedLastCycle==true){//if user has released left click button
				switch(c_mouseCommandState){
				case Selecting://Selection Change
					//pushBackSelection(&c_playerSelection,&s_playerUnits);
					
					grabOnScreenSelectedUnits(&s_playerUnits,&c_playerSelection);
					printf("%d\n",c_playerSelection.size());
					c_selectionDrawState=false;
					break;
				case Commanding:
					CommandSelectionUnits(c_currentCommand,&c_playerSelection);
					break;
				}
			}else{//if user has clicked left click button
				if(mouseIsOnScreen()){
					switch(c_mouseCommandState){
					case Selecting:
						c_clientSelectionShape.setSize(sf::Vector2f(0,0));
						c_originalSelectPoint=sf::Mouse::getPosition(c_window);
						updateSelection();
						c_selectionDrawState=true;
						break;
					case Commanding:
						CommandSelectionUnits(c_currentCommand,&c_playerSelection);
						break;
					}
				}
			}
			c_leftMouseClickedLastCycle^=true;//Swaps leftMouseClickLastCycle state
		}else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){//If Right mbutton clicked
			CommandSelectionUnits(Move,&c_playerSelection);
		}
		
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left)==true &&c_leftMouseClickedLastCycle==true && c_mouseCommandState==Selecting){//If the player is still holding down the left click button while selecting
				updateSelection();
		}
}
int _tmain(int argc, _TCHAR* argv[])
{
	//Client samples input per tick
		//Buffers input 
	//Server simulates gameworld per tick

	//Cheat protection:
	//Encrypt packets
	//or use Base64 and obsfuscate
	
	//Mockup naming conventions
	//c_xxx for client side stuff
	//s_xxx for game state that will be server side in the future
	//Uppercase Snake Case for Macros. Example: ZERG_UNIT_SPEED
	//Type names start with capital letter.
	//Variable names are all lower case.
	//Sensible naming for functions, have them reflect return signature.

	//Game world is a large grid system
	//Units have different shapes

	//TODO Control Groups
	//TODO MAKE Tile constructor
	//TODO Discuss doing the fallout method of 3d.
		//Make render, take isometric snapshots of it. Stick to SFML

	printf("Texture Max Size: %d",sf::Texture().getMaximumSize());
	sf::ContextSettings c_settings;
	c_settings.antialiasingLevel=8;
	
	std::srand(time(NULL));
	for(unsigned int i=0;i<GAMEARRAYSIZE_MOCKUP;i++){
		for(unsigned int j=0;j<GAMEARRAYSIZE_MOCKUP;j++){
			s_gameLevel[i][j]=Tile(std::rand()%2+0);
		}
	}
	//TODO MAKE CLASS FOR GAME UNIT ADDITION TO TILE MANAGER
	//Give tile int cords or on screen float approximations of the tile.
	
	unitManager theUnitManager(&s_playerUnits,&s_enemyUnits,&c_viewPosition,s_gameLevel);
	//for(unsigned int i=0;i<5;i++){
	//	s_playerUnits.push_back(Unit(Point2D((i+1)*150+50,668),sf::Color::Green,10));
	//}
	//for(unsigned int i=0;i<3;i++){
	//	s_enemyUnits.push_back(Unit(Point2D((i+1)*150+180,100),sf::Color::Red,20));
	//}
	//Allows the player to queue up commands.
	//Non Shifted Commands empty the list.
	//Same with Unit Selection

	c_mouseCommandState = Selecting;//True if valid command key is pressed.
	c_leftMouseClickedLastCycle = false;//For box selection

	c_clientSelectionShape.setFillColor(sf::Color::Transparent);
	c_clientSelectionShape.setOutlineThickness(3.0f);
	c_clientSelectionShape.setOutlineColor(sf::Color::Black);
	c_clientSelectionShape.setSize(sf::Vector2f(0.f,0.f));
	c_selectionDrawState = false; //user is selecting if true

	//Click -> Size Box -> Unclick -> Collision Detect Box with Player Units
	//if(shift){add pointers to those Units to Selection Vector}
	//else {empty Selection Vector, add those}
	//Select, Command Key, Left Click

	//Valid Commands: Attack, Move, Halt, Stop
	//LEFT CLICK DOES TWO THINGS. SELECTION AND COMMANDING
	//RIGHT CLICK ONLY DOES MOVE COMMAND

	sf::RectangleShape background(sf::Vector2f(c_window.getSize().x,c_window.getSize().y));
	background.setFillColor(sf::Color::White);

	sf::Texture tileSetTexture;
	if(!tileSetTexture.loadFromFile("mockupTileSet.png")){
		printf("Texture load failure");
		getchar();
		return -1;
	}
	const int tileViewSizeX=c_window.getSize().x/TILE_SIZE;
	const int tileViewSizeY=c_window.getSize().y/TILE_SIZE;
	//printf("\nGame Tile Count: X: %d Y: %d\n",tileViewSizeX,tileViewSizeY);
	sf::VertexArray tileSet(sf::Quads,4*tileViewSizeX*tileViewSizeY);//http://www.sfml-dev.org/tutorials/2.1/graphics-vertex-array.php
	sf::RenderStates tileSetStates;
	tileSetStates.blendMode=sf::BlendNone;
	tileSetStates.texture = &tileSetTexture;

	for (unsigned int i = 0; i < tileViewSizeX; ++i){
            for (unsigned int j = 0; j < tileViewSizeY; ++j){
                sf::Vertex* quad = &tileSet[(i + j * tileViewSizeX) * 4];// get a pointer to the current tile's quad
                setQuadPos(quad,i,j);	 // define its 4 corners
				setQuadTexture(quad,i,j);// define its 4 texture coordinates
           }
	}

	sf::Font fontArial;
	if(!fontArial.loadFromFile("arial.ttf")){
		return -1;
	}

	fpsCounter theFPSCounter(fontArial);

	DrawHandler c_clientDrawHandler;//Put all c_ITEMS with draw calls into the draw handler
    while (c_window.isOpen())
    {
        sf::Event event;
        while (c_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                c_window.close();
        }
		mouseLogic();//Ensure this can be put in client class
        c_window.clear();

		//printf("%s",c_selectionDrawState?"TRUE\n":"FALSE\n");
		c_window.draw(background);

		c_window.draw(tileSet,&tileSetTexture);
		c_window.draw(tileSet,tileSetStates);
		
		drawSelectionStroke(&c_playerSelection);
		drawUnitVector(&c_window,&s_playerUnits);
		drawUnitVector(&c_window,&s_enemyUnits);

		if(c_selectionDrawState==true){
			c_window.draw(c_clientSelectionShape);
		}

		theFPSCounter.updateFPSCounter();
		theFPSCounter.draw(&c_window);
        c_window.display();
		
    }

    return 0;
}
