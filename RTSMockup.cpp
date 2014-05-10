// RTSMockup.cpp : Defines the entry point for the console application.
// TODO comment the dependancies of these headers
#include "stdafx.h"//VS2010 Requirement
#include <string.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Unit.h"//Unit Object Class
#include "Command.h"//Unit Commands
#include "Tile.h"//Game Array Tile Class
#include "Macros.h"//Macros TODO Make config system
#include "fpsCounter.h"//FPS Counter
#include "iostream"
#include "controlPoint.h"
#include "unitSpawner.h"
#include <SFML/Audio.hpp>

enum eGameState {GSM_MENU, GSM_LEVEL, GSM_END};
eGameState gameState;
//#define PLAYER_COLOR Color::Green
//#define SELECTION_STROKE_COLOR sf::Color::Blue
//#define STROKE_SIZE 4
//#define GAMEARRAYSIZE_MOCKUP 128
//#define TILE_SIZE 

sf::View view;
sf::Vector2i screenSize (1024,700);

sf::Vector2i screenPos (0,0);

enum DecisionState {Selecting,Commanding};
 
sf::RectangleShape c_clientSelectionShape;
sf::Vector2i c_originalSelectPoint;//SELECT LEFT CLICK POINT
sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "RTS Mockup");

std::vector<Unit> s_playerUnits;
std::vector<Unit> s_enemyUnits;
std::vector<controlPoint> s_playerOrgans;

Tile gameLevel[GAMEARRAYSIZE_MOCKUP][GAMEARRAYSIZE_MOCKUP];//game level

//Mouse Selection & Commanding
std::vector<Unit *> c_playerSelection;
CommandEnum currentCommand;
//Mouse Logic Variables
bool leftMouseClickedLastCycle;
bool rightMouseClickedLastCycle;
DecisionState mouseCommandState;
bool selectionDrawState;

void drawUnitVector(const std::vector<Unit>& list){//draws all the units in a unit list
	for(unsigned int i=0;i<list.size();i++) {
		if (list[i].HPcurrent >= 0) {
			window.draw(list[i].UnitShape);
			window.draw(list[i].unitHealthBar.HPgreen);
			window.draw(list[i].unitHealthBar.HPred);
		}
	}
}
void drawOrganVector(const std::vector<controlPoint>& list){
	for(unsigned int i = 0; i < list.size();i++){//TODO learn for every in vector loops
		if (list[i].cpHealthBar.HPcurrent >= 0) {
			window.draw(list[i].cpSprite);
			window.draw(list[i].cpHealthBar.HPgreen);
			window.draw(list[i].cpHealthBar.HPred);
		}
	}
}

void updateSelection(){
	sf::Vector2i LimitedMousePos = sf::Mouse::getPosition(window);
	LimitedMousePos.x += screenPos.x;
	LimitedMousePos.y += screenPos.y;
	//if(LimitedMousePos.x<0 || LimitedMousePos.x > window.getSize().x){//if 
	//	if(LimitedMousePos.x<0){
	//		LimitedMousePos.x=0;
	//	}else{
	//		LimitedMousePos.x=window.getSize().x;
	//	}
	//}
	//if(LimitedMousePos.y<0 || LimitedMousePos.y > window.getSize().y){//if 
	//	if(LimitedMousePos.y<0){
	//		LimitedMousePos.y=0;
	//	}else{
	//		LimitedMousePos.y=window.getSize().y;
	//	}
	//}
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
	//for(int i=0;i<4;i++){
		//printf("P(%d): %.0f,%.0f ",i,c_clientSelectionShape.getPoint(i).x+c_clientSelectionShape.getPosition().x,c_clientSelectionShape.getPoint(i).y+c_clientSelectionShape.getPosition().y);
	//}
	//printf("\n\n");

	//printf("Limited MosPos %d %d ",LimitedMousePos.x,LimitedMousePos.y);
	//printf("Real MosPos %d %d\n",sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y);
	/*printf("Original Point %d %d ",c_originalSelectPoint.x,c_originalSelectPoint.y);
	printf("Selection Cords %f.0 %f.0\n\n",c_clientSelectionShape.getPosition().x,c_clientSelectionShape.getPosition().y);*/
}
unsigned int counter=0;
bool listContainsThisPointer(std::vector<Unit *> * list,Unit * pUnit){//TODO make function compliant with remove_if to clean up selection code
	bool returnVal = false;
	if(pUnit!=nullptr && list!=nullptr){
		for(unsigned int i = 0; i < list->size();i++){
			if((*list)[i] == pUnit){
				returnVal = true;
				break;
			}
		}
	}else{
		returnVal;
	}
	return returnVal;
}
float getDist(sf::Vector2f pos1, sf::Vector2f pos2){
	return std::sqrt( std::pow(pos1.x - pos2.x ,2) + std::pow( pos1.y - pos2.y,2));
}
void grabOnScreenSelectedUnits(std::vector<Unit>* s_playerUnits,std::vector<Unit *>* c_playerSelection){//TODO reimplement the extra special functionality. Click on units. Select if box within radius of unit
	bool shifted = false;
	unsigned int preExistingElemCount=0;
	if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)&&!sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)){
		printf("\nUnShifted %S Command\n","Selection");
		(*c_playerSelection).clear();
		//printf("\nList cleared\n");
	}else{
		printf("\nShifted %S Command\n","Selection");
		shifted=true;
		preExistingElemCount=(*c_playerSelection).size();
	}
	float minX=window.getSize().x;
	float maxX=0.f;
	float minY=window.getSize().y;
	float maxY=0.f;
	for(int i=0;i<4;i++){//Iterate through selection points and get the bounds of the box.
		minX = minX > c_clientSelectionShape.getPoint(i).x+c_clientSelectionShape.getPosition().x ? c_clientSelectionShape.getPoint(i).x+c_clientSelectionShape.getPosition().x : minX;
		maxX = maxX < c_clientSelectionShape.getPoint(i).x+c_clientSelectionShape.getPosition().x ? c_clientSelectionShape.getPoint(i).x+c_clientSelectionShape.getPosition().x : maxX;
		minY = minY > c_clientSelectionShape.getPoint(i).y+c_clientSelectionShape.getPosition().y ? c_clientSelectionShape.getPoint(i).y+c_clientSelectionShape.getPosition().y : minY;
		maxY = maxY < c_clientSelectionShape.getPoint(i).y+c_clientSelectionShape.getPosition().y ? c_clientSelectionShape.getPoint(i).y+c_clientSelectionShape.getPosition().y : maxY;
	}
	int diffX = maxX - minX;
	int diffY = maxY - minY;


	float radius=0.f;
	for(unsigned int i = 0; i < s_playerUnits->size(); i++){
		Unit * pUnit = &(*s_playerUnits)[i];
		radius = pUnit->UnitShape.getRadius();

		
			sf::Vector2f unitPos = pUnit->UnitShape.getPosition();
			unitPos.x+= radius;
			unitPos.y+= radius;
			float dist = getDist(unitPos, c_clientSelectionShape.getPosition());
			//printf("\n%f\n",dist);
			if(diffX <=1 && diffY <= 1){
				if(dist <= pUnit->UnitShape.getRadius()){
					if(listContainsThisPointer(c_playerSelection,pUnit) == false){
						c_playerSelection->push_back(pUnit);
						break;
						//printf("Original Point %d %d",originalPoint.x,originalPoint.y);
						//printf("Selection Cords %d %d\n",selectionShape.getPosition().x,selectionShape.getPosition().y);
					}
				}
			}else if(pUnit->UnitShape.getPosition().x <= (maxX + radius) && pUnit->UnitShape.getPosition().x >= (minX - radius)){
				if(pUnit->UnitShape.getPosition().y <= (maxY + radius) && pUnit->UnitShape.getPosition().y >= (minY - radius)){
					if(listContainsThisPointer(c_playerSelection,pUnit) == false){
						c_playerSelection->push_back(pUnit);

					}
				}else{
					if(!shifted && listContainsThisPointer(c_playerSelection,pUnit) == true){
						c_playerSelection->erase( std::remove(std::begin(*c_playerSelection), std::end(*c_playerSelection), pUnit), std::end(*c_playerSelection));
					}
				}
			}else{
				if(!shifted && listContainsThisPointer(c_playerSelection,pUnit) == true){
					c_playerSelection->erase( std::remove(std::begin(*c_playerSelection), std::end(*c_playerSelection), pUnit), std::end(*c_playerSelection));
				}
			}
		}
		printf("\nUnit Count: %d\n",c_playerSelection->size());
	//Translation of the view must be incorporated
	//TODO FIX SELECTION
	//UNDO THIS to demonstrate
	//c_playerSelection->push_back(&(*s_playerUnits)[0]);
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
Unit * unitAtMousePos(){//TODO: FINISH THIS FUNCTION check for target unit at mouse posisiton
	//check for target unit at mouse posisiton
	return NULL;
}
void CommandSelectionUnits(CommandEnum theCommand, std::vector<Unit *> * c_playerSelection){//TODO implement other commands
	if(theCommand==Move){
		for(unsigned int i=0;i<c_playerSelection->size();i++){
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)&&!sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)){
				printf("UnShifted %S Command",commandString(theCommand));
				(*c_playerSelection)[i]->unitCommands.clear();
				//(*c_playerSelection)[i]->unitCommands.empty();
			}else{
				printf("Shifted %S Command",commandString(theCommand));
			}
			sf::Vector2i tempMousePos = sf::Mouse::getPosition(window);
			tempMousePos.x += screenPos.x;//TODO make translation mouse cord
			tempMousePos.y += screenPos.y;
			if(tempMousePos.x < 0){
				tempMousePos.x = 1;
			}
			if(tempMousePos.y < 0){
				tempMousePos.y = 1;
			}
			Command tempComand(Command(Move,tempMousePos,unitAtMousePos()));
			(*(*c_playerSelection)[i]).unitCommands.push_back(tempComand);
		}
	}else{
		printf("No...\n");
	}
}


bool mouseIsOnScreen(){
	//printf("%d %d\n",sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y);
	if((sf::Mouse::getPosition().x-window.getPosition().x >0) && (sf::Mouse::getPosition(window).x <=window.getSize().x)){
		if((sf::Mouse::getPosition().y-window.getPosition().y >0) && (sf::Mouse::getPosition(window).y <=window.getSize().y)){
			//printf("\nOnscreen\n");
			return true;
		}
	}
	return false;
}
void drawSelectionStroke(std::vector<Unit*>* c_playerSelection){
	sf::CircleShape strokeShape(0.f);
	strokeShape.setFillColor(sf::Color::Blue);
	//printf(" %d\n",(*c_playerSelection).size());
	for(unsigned int i=0;i<(*c_playerSelection).size();i++){
		strokeShape.setRadius((*(*c_playerSelection)[i]).UnitShape.getRadius()+STROKE_SIZE);
		strokeShape.setPosition((*(*c_playerSelection)[i]).UnitShape.getPosition().x-STROKE_SIZE,(*(*c_playerSelection)[i]).UnitShape.getPosition().y-STROKE_SIZE);
		window.draw(strokeShape);
	}
}
void setQuadPos(sf::Vertex * quad,const unsigned int i,const unsigned int j){
	quad[0].position = sf::Vector2f(i * TILE_SIZE, j * TILE_SIZE);
    quad[1].position = sf::Vector2f((i + 1) * TILE_SIZE, j * TILE_SIZE);
    quad[2].position = sf::Vector2f((i + 1) * TILE_SIZE, (j + 1) * TILE_SIZE);
    quad[3].position = sf::Vector2f(i * TILE_SIZE, (j + 1) * TILE_SIZE);
}

void setQuadTexture(sf::Vertex * quad,const unsigned int i,const unsigned int j){//TODO FIX THIS
	//printf("\n\n");
	//	FALSE		TRUE
	//0,0  32,0		32,0  64,0
	//0,32 32,32	32,32 64,32
	sf::Vector2f cordOrder[4]={ sf::Vector2f((gameLevel[i][j].open*TILE_SIZE)+0.0,0.0),
								sf::Vector2f((gameLevel[i][j].open*TILE_SIZE)+TILE_SIZE,0.0),
								sf::Vector2f((gameLevel[i][j].open*TILE_SIZE)+TILE_SIZE,TILE_SIZE),
								sf::Vector2f((gameLevel[i][j].open*TILE_SIZE)+0.0,TILE_SIZE)
	};
	quad[0].texCoords = cordOrder[0];//TOP LEFT
	quad[1].texCoords = cordOrder[1];//TOP RIGHT
	quad[2].texCoords = cordOrder[2];//BOTTOM RIGHT
	quad[3].texCoords = cordOrder[3];//BOTTOM LEFT
	//if(gameLevel[i][j].open){
		//printf("%s",gameLevel[i][j].open?"TRUE\n":"FALSE\n");
		//printf("TOP LEFT %f %f\n",quad[0].texCoords.x,quad[0].texCoords.y);
		//printf("TOP RIGHT %f %f\n",quad[1].texCoords.x,quad[1].texCoords.y);
		//printf("BOTTOM RIGHT %f %f\n",quad[2].texCoords.x,quad[2].texCoords.y);
		//printf("BOTTOM LEFT %f %f\n",quad[3].texCoords.x,quad[3].texCoords.y);
		//printf("\n");
	//}
}
void mouseLogic(){
		//IF MOUSE IS ON THE WINDOW
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)^leftMouseClickedLastCycle) {//if Left Mouse Button state has changed since last cycle
			if (leftMouseClickedLastCycle == true) {//if user has released left click button
				switch(mouseCommandState){
				case Selecting://Selection Change
					//pushBackSelection(&c_playerSelection,&s_playerUnits);
					
					grabOnScreenSelectedUnits(&s_playerUnits,&c_playerSelection);
					//printf("%d\n",c_playerSelection.size());
					selectionDrawState=false;
					break;
				case Commanding:
					CommandSelectionUnits(currentCommand,&c_playerSelection);
					break;
				}
			}else{//if user has clicked left click button
				if(mouseIsOnScreen()){
					switch(mouseCommandState){
					case Selecting:
						c_clientSelectionShape.setSize(sf::Vector2f(0,0));
						c_originalSelectPoint=sf::Mouse::getPosition(window);
						c_originalSelectPoint.x += screenPos.x;
						c_originalSelectPoint.y += screenPos.y;
						updateSelection();
						selectionDrawState=true;
						break;
					case Commanding:
						CommandSelectionUnits(currentCommand,&c_playerSelection);
						break;
					}
				}
			}
			leftMouseClickedLastCycle^=true;//Swaps leftMouseClickLastCycle state
		}else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)^rightMouseClickedLastCycle){//If Right mbutton clicked
			CommandSelectionUnits(Move,&c_playerSelection);
			rightMouseClickedLastCycle^=true;
		}
		
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left)==true &&leftMouseClickedLastCycle==true && mouseCommandState==Selecting){//If the player is still holding down the left click button while selecting
					updateSelection();
		}
}
void updateMoveLogic(std::vector<Unit> * list){
	for(unsigned int i = 0; i < list->size(); i++){
		if ((*list)[i].unitCommands.size()!=0) {//TODO make this a function 
			if ((*list)[i].unitCommands[0].theCommand == Move) {
				double angle = 0;
				sf::Vector2i currentPos = (*list)[i].unitCommands[0].mousePosition;
				sf::Vector2i castedPos((*list)[i].UnitShape.getPosition().x,(*list)[i].UnitShape.getPosition().y);
				if ((*list)[i].unitCommands[0].mousePosition.x - (*list)[i].UnitShape.getPosition().x >= 0) {
					angle = atan(((float) currentPos.y - castedPos.y) / (currentPos.x - castedPos.x));
					(*list)[i].UnitShape.move(cos(angle), sin(angle));
				} else {
					angle = atan(((*list)[i].unitCommands[0].mousePosition.y - (*list)[i].UnitShape.getPosition().y) / ((*list)[i].UnitShape.getPosition().x - (*list)[i].unitCommands[0].mousePosition.x));
					(*list)[i].UnitShape.move(-1 * cos(angle), sin(angle));
				}
				if( (castedPos.x > (currentPos.x - 1) && castedPos.x < (currentPos.x + 1)) && (castedPos.y > (currentPos.y - 1) && castedPos.y < (currentPos.y + 1)))
				{
					(*list)[i].unitCommands.erase((*list)[i].unitCommands.begin());
				}
			}
		}
	}
}
void updateUnitHealthBars(std::vector<Unit> * list){
	for (unsigned int i=0; i<list->size(); i++) {
		s_enemyUnits[i].unitHealthBar.HPupdate(s_enemyUnits[i].UnitShape.getPosition(),&s_enemyUnits[i].HPcurrent);
	}
}
void updateOrganHealthBars(void){
	for ( unsigned int i = 0; i < s_playerOrgans.size(); i++){
		sf::Vector2f pos = s_playerOrgans[i].cpSprite.getPosition();
		const sf::Texture * pTexture = s_playerOrgans[i].cpSprite.getTexture();
		pos.x += pTexture->getSize().x;
		pos.y += pTexture->getSize().y;
		s_playerOrgans[i].cpHealthBar.HPupdate(pos,&s_playerOrgans[i].HPcurrent);
	}
}
void doPlayerDamage(void){
#define ATTACK_RANGE_MACRO 5
	for(unsigned int i = 0; i < s_playerUnits.size(); i++){
		for(unsigned int j = 0; j < s_enemyUnits.size(); j++){
			float dist = std::sqrt(std::pow((s_enemyUnits[j].UnitShape.getPosition().x + s_enemyUnits[j].UnitShape.getRadius()) - (s_playerUnits[i].UnitShape.getPosition().x + s_playerUnits[i].UnitShape.getRadius()), 2) + std::pow((s_enemyUnits[j].UnitShape.getPosition().y + s_enemyUnits[j].UnitShape.getRadius()) - (s_playerUnits[i].UnitShape.getPosition().y + s_playerUnits[i].UnitShape.getRadius()), 2));
			if (dist < ATTACK_RANGE_MACRO * s_playerUnits[i].UnitShape.getRadius()) {
				s_enemyUnits[j].HPcurrent = s_enemyUnits[j].HPcurrent - 1;
			}
		}
	}
}
void gameLogic() {
	updateMoveLogic(&s_enemyUnits);
	updateMoveLogic(&s_playerUnits);
	
	doPlayerDamage();

	updateUnitHealthBars(&s_enemyUnits);
	updateUnitHealthBars(&s_playerUnits);
	updateOrganHealthBars();

}
void menu()
{


	sf::RectangleShape rectangle(sf::Vector2f(120, 50));
	rectangle.setSize(sf::Vector2f(400, 100));
	rectangle.setPosition(300,300);
	rectangle.setFillColor(sf::Color::Magenta);

	sf::Texture texture;

	if(!texture.loadFromFile("MainMenu.png"))
	{
		printf("error loading picture\n");
	}

	sf::Sprite sprite;
	sprite.setTexture(texture);

	while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
			sf::Vector2i point = sf::Mouse::getPosition(window);

			if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				if(point.x>=380&&point.x<=633)
				{
					if(point.y>=325&&point.y<=415)
					{
						gameState=GSM_LEVEL;
						break;
					}
				}
			}

        window.clear(sf::Color::Black);
		window.draw(rectangle);
		window.draw(sprite);
        window.display();
    }
}
int mainGame(){
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
	
	//spawnerFriendly.png
	//spawnerRed.png

	//sf::Sprite testSprite;
	//testSprite.setPosition(20,20);
	//testSprite.setTexture(lungLeft);

	sf::Music music;
	music.openFromFile("rtsGame.wav");


	printf("Texture Max Size: %d",sf::Texture().getMaximumSize());
	sf::ContextSettings c_settings;
	c_settings.antialiasingLevel=8;

	std::srand(std::time(nullptr));

	const Unit friendlyUnitTemplate = Unit(Point2D(0,0),sf::Color::Green,10);
	const Unit enemyUnitTemplate = Unit(Point2D(0,0),sf::Color::Red,20);

	sf::Texture friendlySpawnerTexture;
	sf::Texture enemySpawnerTexture;
	if(!friendlySpawnerTexture.loadFromFile("spawnerFriendly.png")){
		printf("spawnerFriendly.png load failed.");
	}
	if(!enemySpawnerTexture.loadFromFile("spawnerRed.png")){
		printf("spawnerRed.png.png load failed.");
	}

	unitSpawner friendlySpawner = unitSpawner(&s_playerUnits,friendlyUnitTemplate,sf::Vector2i(window.getSize().x/2,500),&friendlySpawnerTexture);
	unitSpawner enemySpawner = unitSpawner(&s_enemyUnits,enemyUnitTemplate,sf::Vector2i(window.getSize().x/2,100),&enemySpawnerTexture);
	friendlySpawner.spawnUnit(10,80);
	enemySpawner.spawnUnit(10,80);

	for(unsigned int i=0;i<GAMEARRAYSIZE_MOCKUP;i++){
		for(unsigned int j=0;j<GAMEARRAYSIZE_MOCKUP;j++){
			gameLevel[i][j]=Tile(std::rand()%2+0);
			//printf("%s\n",gameLevel[i][j].open?"TRUE":"FALSE");
			//gameLevel[i][j].tileSprite.setPosition(sf::Vector2f(i*32,j*32));
		}
	}
	sf::Texture lungLeft;
	sf::Texture lungRight;
	sf::Texture heart;
	if(!lungLeft.loadFromFile("lungSpriteLeft.png")){
		printf("lungSpriteLeft.png load fail");
	}
	if(!lungRight.loadFromFile("lungSpriteRight.png")){
		printf("lungSpriteRight.png load fail");
	}
	if(!heart.loadFromFile("smallheart.png")){
		printf("heart load fail");
	}


	s_playerOrgans.push_back(controlPoint(&lungLeft,sf::Vector2f(300,300)));
	s_playerOrgans.push_back(controlPoint(&lungRight,sf::Vector2f(700,100)));
	s_playerOrgans.push_back(controlPoint(&heart,sf::Vector2f(500,300)));
	//Allows the player to queue up commands.
	//Non Shifted Commands empty the list.
	//Same with Unit Selection

	mouseCommandState = Selecting; //True if valid command key is pressed.
	leftMouseClickedLastCycle = false; //For box selection
	rightMouseClickedLastCycle = false;

	c_clientSelectionShape.setFillColor(sf::Color::Transparent);
	c_clientSelectionShape.setOutlineThickness(3.0f);
	c_clientSelectionShape.setOutlineColor(sf::Color::Black);
	c_clientSelectionShape.setSize(sf::Vector2f(0.f,0.f));
	selectionDrawState = false; //user is selecting if true

	//Click -> Size Box -> Unclick -> Collision Detect Box with Player Units
	//if(shift){add pointers to those Units to Selection Vector}
	//else {empty Selection Vector, add those}
	//Select, Command Key, Left Click

	//Valid Commands: Attack, Move, Halt, Stop
	//LEFT CLICK DOES TWO THINGS. SELECTION AND COMMANDING
	//RIGHT CLICK ONLY DOES MOVE COMMAND

	sf::RectangleShape background(sf::Vector2f(window.getSize().x,window.getSize().y));
	background.setFillColor(sf::Color::White);

	sf::Texture tileSetTexture;
	if(!tileSetTexture.loadFromFile("mockupTileSet.png")){
		printf("Texture load failure");
		getchar();
		return -1;
	}
	const int tileViewSizeX=window.getSize().x/TILE_SIZE;
	const int tileViewSizeY=window.getSize().y/TILE_SIZE;
	//printf("\nGame Tile Count: X: %d Y: %d\n",tileViewSizeX,tileViewSizeY);
	sf::VertexArray tileSet(sf::Quads,4*tileViewSizeX*tileViewSizeY);//http://www.sfml-dev.org/tutorials/2.1/graphics-vertex-array.php
	sf::RenderStates tileSetStates;
	
	fpsCounter myFps;
	tileSetStates.blendMode=sf::BlendNone;
	tileSetStates.texture = &tileSetTexture;

	for (unsigned int i = 0; i < tileViewSizeX; ++i){
            for (unsigned int j = 0; j < tileViewSizeY; ++j){
				// get a pointer to the current tile's quad
                sf::Vertex* quad = &tileSet[(i + j * tileViewSizeX) * 4];
                setQuadPos(quad,i,j);	 // define its 4 corners
				setQuadTexture(quad,i,j);// define its 4 texture coordinates
           }
	}
	sf::Font fontArial;
	if(!fontArial.loadFromFile("arial.ttf")){
		return -1;
	}
	fpsCounter theFPSCounter(fontArial);
	sf::Texture gabeBackground;
	if(!gabeBackground.loadFromFile("bodyBGsmall.png")){
		printf("load failure");
	}
	sf::Sprite bgSprite(gabeBackground);
	music.play();

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
		mouseLogic();
		gameLogic();
        window.clear();
		float delta = myFps.getDelta();
		
		if(sf::Mouse::getPosition(window).x > screenSize.x)
		{
			screenPos.x += 10*delta;
		}
		else if(sf::Mouse::getPosition(window).x < 0)
		{
			screenPos.x -=10*delta;
		}
		if(sf::Mouse::getPosition(window).y > screenSize.y)
		{
			screenPos.y +=10*delta;
		}
		else if(sf::Mouse::getPosition(window).y < 0)
		{
			screenPos.y -=10*delta;
		}
		
		sf::FloatRect tempRect = sf::FloatRect(screenPos.x,screenPos.y,screenSize.x,screenSize.y);
		view.reset(tempRect);
		sf::Vector2f test = view.getCenter();
		//printf("\nTranslation Cords:%f,%f\n",test.x,test.y);
		//printf("\nUnit 0 Cord:%f,%f",s_playerUnits[0].UnitShape.getPosition().x,s_playerUnits[0].UnitShape.getPosition().y);
		window.setView(view);
		//std::cout << screenPos.x << " " << screenPos.y << std::endl;

		//printf("%s",selectionDrawState?"TRUE\n":"FALSE\n");

		window.draw(background);

		window.draw(tileSet,&tileSetTexture);
		window.draw(tileSet,tileSetStates);

		window.draw(bgSprite);

		friendlySpawner.draw(&window);
		enemySpawner.draw(&window);

		drawOrganVector(s_playerOrgans);

		drawSelectionStroke(&c_playerSelection);
		drawUnitVector(s_playerUnits);
		drawUnitVector(s_enemyUnits);

		if(selectionDrawState==true){
			window.draw(c_clientSelectionShape);
		}
		//window.draw(testSprite);



		theFPSCounter.updateFPSCounter();
		theFPSCounter.draw(&window,&screenPos);
	
		window.draw(s_playerOrgans[0].cpSprite);
        window.display();
		
    }
}


int _tmain(int argc, _TCHAR* argv[])
{
	gameState = GSM_MENU;

	while(gameState != GSM_END){
		switch (gameState)
		{
		  case GSM_MENU :
		  {
			// Menu state
			  menu();
			/*if(menu()==1)
			{
				gameState = GSM_LEVEL;
			}*/
			  break;
		  }
		  
		  case GSM_LEVEL :
		  {
			// Level state
			  mainGame();
			  gameState = GSM_END;
			  break;
		  }
		 
		  case GSM_END :
		  {
			// End state
			   break;
		  }
		 
		}
	}
    return 0;
}
