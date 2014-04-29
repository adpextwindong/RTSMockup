// RTSMockup.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Unit.h"//Unit Object Class
#include "Command.h"//Unit Commands
#include "Tile.h"//Game Array Tile Class
#include "Macros.h"//Macros TODO Make config system
#include "fpsCounter.h"//FPS Counter
#include "iostream"

//#define PLAYER_COLOR Color::Green
//#define SELECTION_STROKE_COLOR sf::Color::Blue
//#define STROKE_SIZE 4
//#define GAMEARRAYSIZE_MOCKUP 128
//#define TILE_SIZE 32

enum DecisionState {Selecting,Commanding};
 
sf::RectangleShape c_clientSelectionShape;
sf::Vector2i c_originalSelectPoint;//SELECT LEFT CLICK POINT
sf::RenderWindow window(sf::VideoMode(1024, 768), "RTS Mockup");
std::vector<Unit> s_playerUnits;
std::vector<Unit> s_enemyUnits;
Tile gameLevel[GAMEARRAYSIZE_MOCKUP][GAMEARRAYSIZE_MOCKUP];//game level



//Mouse Selection & Commanding
std::vector<Unit *> c_playerSelection;
CommandEnum currentCommand;
//Mouse Logic Variables
bool leftMouseClickedLastCycle;
bool rightMouseClickedLastCycle;
DecisionState mouseCommandState;
bool selectionDrawState;

void drawUnitVector(sf::RenderWindow * window, const std::vector<Unit>& list){//draws all the units in a unit list
	for(unsigned int i=0;i<list.size();i++){
		(*window).draw(list[i].UnitShape);
	}
	
}

void updateSelection(){
	sf::Vector2i LimitedMousePos = sf::Mouse::getPosition(window);
	if(LimitedMousePos.x<0 || LimitedMousePos.x > window.getSize().x){//if 
		if(LimitedMousePos.x<0){
			LimitedMousePos.x=0;
		}else{
			LimitedMousePos.x=window.getSize().x;
		}
	}
	if(LimitedMousePos.y<0 || LimitedMousePos.y > window.getSize().y){//if 
		if(LimitedMousePos.y<0){
			LimitedMousePos.y=0;
		}else{
			LimitedMousePos.y=window.getSize().y;
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
	//printf("Real MosPos %d %d\n",sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y);
	/*printf("Original Point %d %d ",c_originalSelectPoint.x,c_originalSelectPoint.y);
	printf("Selection Cords %f.0 %f.0\n\n",c_clientSelectionShape.getPosition().x,c_clientSelectionShape.getPosition().y);*/
}
unsigned int counter=0;
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
				sf::Vector2f position((*s_playerUnits)[unitIterator].position.x,(*s_playerUnits)[unitIterator].position.y);
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
	return NULL;
}
void CommandSelectionUnits(CommandEnum theCommand, std::vector<Unit *> * c_playerSelection){
	if(theCommand==Move){
		for(unsigned int i=0;i<c_playerSelection->size();i++){
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)&&!sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)){
				printf("UnShifted %S Command",commandString(theCommand));
				(*c_playerSelection)[i]->unitCommands.empty();
			}
			printf("Shifted %S Command",commandString(theCommand));
			Command tempComand(Command(Move,sf::Mouse::getPosition(window),unitAtMousePos()));
			if(tempComand.theCommand == Move){
				printf("its equal to move");
			}
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
	strokeShape.setFillColor(SELECTION_STROKE_COLOR);
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
	printf("\n\n");
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
//void drawLevel(){ DEPRECATED FOR VERTEX ARRAY
//	for(unsigned int i=0;i<GAMEARRAYSIZE_MOCKUP;i++){
//		for(unsigned int j=0;j<GAMEARRAYSIZE_MOCKUP;j++){
//			window.draw(gameLevel[i][j].tileSprite);
//		}
//	}
//}
void mouseLogic(){
		//IF MOUSE IS ON THE WINDOW
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)^leftMouseClickedLastCycle) {//if Left Mouse Button state has changed since last cycle
			if (leftMouseClickedLastCycle == true) {//if user has released left click button
				switch(mouseCommandState){
				case Selecting://Selection Change
					//pushBackSelection(&c_playerSelection,&s_playerUnits);
					
					grabOnScreenSelectedUnits(&s_playerUnits,&c_playerSelection);
					printf("%d\n",c_playerSelection.size());
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

void gameLogic() {
	for (unsigned int i=0; i<5; i++) {
		if (s_playerUnits[i].unitCommands.size()!=0) {
			if (s_playerUnits[i].unitCommands[0].theCommand == Move) {
				//double angle = atan((s_playerUnits[i].unitCommands[0].mousePosition.y - s_playerUnits[i].UnitShape.getPosition().y) - (s_playerUnits[i].unitCommands[0].mousePosition.x - s_playerUnits[i].UnitShape.getPosition().x));
				double angle = 0;
				sf::Vector2i currentPos = s_playerUnits[i].unitCommands[0].mousePosition;
				////sf::Vector2f destinationPos =  s_playerUnits[0].UnitShape.getPosition();
				sf::Vector2i castedPos(s_playerUnits[0].UnitShape.getPosition().x,s_playerUnits[0].UnitShape.getPosition().y);
				/*if( (currentPos.x > (castedPos.x - 1) || currentPos.x < (castedPos.x + 1)) && (currentPos.y > (castedPos.y - 1) || currentPos.y < (castedPos.y + 1)))
				{
					s_playerUnits[i].unitCommands.erase(s_playerUnits[i].unitCommands.begin());
				}else{*/
				//s_playerUnits[i].unitCommands.erase(s_playerUnits[i].unitCommands.begin());
				if (s_playerUnits[i].unitCommands[0].mousePosition.x - s_playerUnits[i].UnitShape.getPosition().x >= 0) {
					angle = atan(((float) currentPos.y - castedPos.y) / (currentPos.x - castedPos.x));
					//angle = atan((s_playerUnits[i].unitCommands[0].mousePosition.y - s_playerUnits[i].UnitShape.getPosition().y) / (s_playerUnits[i].unitCommands[0].mousePosition.x - s_playerUnits[i].UnitShape.getPosition().x));
					s_playerUnits[i].UnitShape.move(cos(angle), sin(angle));
				} else {
					angle = atan((s_playerUnits[i].unitCommands[0].mousePosition.y - s_playerUnits[i].UnitShape.getPosition().y) / (s_playerUnits[i].UnitShape.getPosition().x - s_playerUnits[i].unitCommands[0].mousePosition.x));
					//angle = atan((s_playerUnits[i].unitCommands[0].mousePosition.y - s_playerUnits[i].UnitShape.getPosition().y) / (s_playerUnits[i].unitCommands[0].mousePosition.x - s_playerUnits[i].UnitShape.getPosition().x));
					s_playerUnits[i].UnitShape.move(-1 * cos(angle), sin(angle));
					//s_playerUnits[i].UnitShape.move(cos(angle), sin(angle));
				}
				//s_playerUnits[i].UnitShape.move(sf::Mouse::getPosition(window).x - s_playerUnits[i].UnitShape.getPosition().x, sf::Mouse::getPosition(window).y - s_playerUnits[i].UnitShape.getPosition().y);
				//std::cout << angle << std::endl;

				//}
				//tan^-1((y_mouse - y_unit) / (x_mouse - x_unit))
				//x_unit = x_unit + 10cos(angle)
				//y_unit = y_unit + 10cos(angle)
			}

		}
	}

	/*for (int i=0; i<5; i++) {
		for (int j=0; j<5; j++) {
			if (std::sqrt(std::pow(s_enemyUnits[j].UnitShape.getPosition().x - s_playerUnits[j].UnitShape.getPosition().x, 2) +  std::pow(s_enemyUnits[j].UnitShape.getPosition().y - s_playerUnits[j].UnitShape.getPosition().y, 2)) > s_playerUnits[i].UnitShape.getRadius()) {
				s_enemyUnits[j].HPcurrent = s_enemyUnits[j].HPcurrent - 5;
			}
		}
	}*/
}

int _tmain(int argc, _TCHAR* argv[])
{
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
			gameLevel[i][j]=Tile(std::rand()%2+0);
			//printf("%s\n",gameLevel[i][j].open?"TRUE":"FALSE");
			//gameLevel[i][j].tileSprite.setPosition(sf::Vector2f(i*32,j*32));
		}
	}
	//TODO MAKE CLASS FOR GAME UNIT ADDITION TO TILE MANAGER
	//Give tile int cords or on screen float approximations of the tile.
	for(unsigned int i=0;i<5;i++){
		s_playerUnits.push_back(Unit(Point2D((i+1)*150+50,668),sf::Color::Green,10));
	}
	for(unsigned int i=0;i<3;i++){
		s_enemyUnits.push_back(Unit(Point2D((i+1)*150+180,100),sf::Color::Red,20));
	}
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

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
		mouseLogic();
		gameLogic();
        window.clear();

		//printf("%s",selectionDrawState?"TRUE\n":"FALSE\n");
		window.draw(background);

		window.draw(tileSet,&tileSetTexture);
		window.draw(tileSet,tileSetStates);
		
		drawSelectionStroke(&c_playerSelection);
		drawUnitVector(&window,s_playerUnits);
		drawUnitVector(&window,s_enemyUnits);

		if(selectionDrawState==true){
			window.draw(c_clientSelectionShape);
		}

		theFPSCounter.updateFPSCounter();
		theFPSCounter.draw(&window);
        window.display();
		
    }

    return 0;
}
