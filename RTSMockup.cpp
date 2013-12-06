// RTSMockup.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <string.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Unit.h"//Unit Object Class
#include "Command.h"//Unit Commands

#define PLAYER_COLOR Color::Green
#define SELECTION_STROKE_COLOR sf::Color::Blue
#define STROKE_SIZE 4

enum DecisionState {Selecting,Commanding};
 
sf::RectangleShape selectionShape;
sf::Vector2i originalPoint;//SELECT LEFT CLICK POINT
sf::RenderWindow window(sf::VideoMode(1024, 768), "RTS Mockup");

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
	int yDiff=(LimitedMousePos.y-originalPoint.y);
	if((LimitedMousePos.x-originalPoint.x)<=0){//Xdiff +x
		if(yDiff<=0){//+x +y Quad 1
			selectionShape.setPosition(originalPoint.x,LimitedMousePos.y);
			selectionShape.setSize(sf::Vector2f(LimitedMousePos.x-originalPoint.x,originalPoint.y-LimitedMousePos.y));
		}else{//+x -y Quad 4
			selectionShape.setSize(sf::Vector2f(LimitedMousePos.x-originalPoint.x,LimitedMousePos.y-originalPoint.y));
		}
	}else{
		if(yDiff<=0){// -x +y Quad 2
			selectionShape.setPosition(sf::Vector2f(LimitedMousePos));
			selectionShape.setSize(sf::Vector2f(originalPoint.x-LimitedMousePos.x,originalPoint.y-LimitedMousePos.y));
		}else{// -x -y Quad 3
			selectionShape.setPosition(sf::Vector2f(LimitedMousePos.x,originalPoint.y));
			selectionShape.setSize(sf::Vector2f(originalPoint.x-LimitedMousePos.x,LimitedMousePos.y-originalPoint.y));
		}
	}

	printf("Shape Pos:%f %f\n",selectionShape.getPosition().x,selectionShape.getPosition().y);
	for(int i=0;i<4;i++){
		printf("P(%d): %.0f,%.0f ",i,selectionShape.getPoint(i).x+selectionShape.getPosition().x,selectionShape.getPoint(i).y+selectionShape.getPosition().y);
	}
	printf("\n\n");
	//printf("Limited MosPos %d %d ",LimitedMousePos.x,LimitedMousePos.y);
	//printf("Real MosPos %d %d\n",sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y);
	/*printf("Original Point %d %d ",originalPoint.x,originalPoint.y);
	printf("Selection Cords %f.0 %f.0\n\n",selectionShape.getPosition().x,selectionShape.getPosition().y);*/
}
void grabOnScreenSelectedUnits(std::vector<Unit>* playerUnits,std::vector<Unit *>* playerSelection){
	bool shifted = false;
	unsigned int preExistingElemCount=0;
	if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)&&!sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)){
		printf("UnShifted %S Command","Selection");
		(*playerSelection).clear();
		//printf("\nList cleared\n");
	}else{
		printf("Shifted %S Command","Selection");
		shifted=true;
		preExistingElemCount=(*playerSelection).size();
	}
	float minX=selectionShape.getPoint(0).x+selectionShape.getPosition().x;
	float maxX=0.f;
	float minY=selectionShape.getPoint(0).y+selectionShape.getPosition().y;
	float maxY=0.f;
	for(int i=0;i<4;i++){
		minX = minX > selectionShape.getPoint(i).x+selectionShape.getPosition().x ? selectionShape.getPoint(i).x+selectionShape.getPosition().x : minX;
		maxX = maxX < selectionShape.getPoint(i).x+selectionShape.getPosition().x ? selectionShape.getPoint(i).x+selectionShape.getPosition().x : maxX;
		minY = minY > selectionShape.getPoint(i).y+selectionShape.getPosition().y ? selectionShape.getPoint(i).y+selectionShape.getPosition().y : minY;
		maxY = maxY < selectionShape.getPoint(i).y+selectionShape.getPosition().y ? selectionShape.getPoint(i).y+selectionShape.getPosition().y : maxY;
	}
	float radius=0.f;
	for(unsigned int i=0;i<(*playerUnits).size();i++){
		Unit * currentPointer=NULL;
		bool containsThePointer = false;
		if(shifted==true){//Checks vector if it contains pointer, allows for no duplicates
			currentPointer =&(*playerUnits)[i];
			for(unsigned int j=0;j<preExistingElemCount;j++){
				if(currentPointer==(*playerSelection)[j]){
					containsThePointer=true;
					break;
				}
			}
		}
		if(containsThePointer==false){//adds the pointer to the list if its in the selection box
			if(selectionShape.getSize().x ==0 && selectionShape.getSize().y ==0){
				if(sqrt(std::pow((*playerUnits)[i].UnitShape.getRadius()+(*playerUnits)[i].UnitShape.getPosition().x-selectionShape.getPosition().x,2.0f)+std::pow((*playerUnits)[i].UnitShape.getRadius()+(*playerUnits)[i].UnitShape.getPosition().y-selectionShape.getPosition().y,2.0f))<=(*playerUnits)[i].UnitShape.getRadius()){
					(*playerSelection).push_back(&(*playerUnits)[i]);
				}
			}else{
				radius=(*playerUnits)[i].UnitShape.getRadius()/2;
				sf::Vector2f position((*playerUnits)[i].position.x,(*playerUnits)[i].position.y);
				if(position.x >= minX && position.x <= maxX){
					if(position.y >= minY && position.y <= maxY){
						(*playerSelection).push_back(&(*playerUnits)[i]);
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
void CommandSelectionUnits(CommandEnum theCommand, std::vector<Unit *> * playerSelection){
	for(unsigned int i=0;i<(*playerSelection).size();i++){
		if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)&&!sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)){
			printf("UnShifted %S Command",commandString(theCommand));
			(*(*playerSelection)[i]).unitCommands.empty();
		}
		printf("Shifted %S Command",commandString(theCommand));
		(*(*playerSelection)[i]).unitCommands.push_back(Command(theCommand,Point2D(sf::Mouse::getPosition(window)),unitAtMousePos()));
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
void drawSelectionStroke(std::vector<Unit*>* playerSelection){
	sf::CircleShape strokeShape(0.f);
	strokeShape.setFillColor(SELECTION_STROKE_COLOR);
	//printf(" %d\n",(*playerSelection).size());
	for(unsigned int i=0;i<(*playerSelection).size();i++){
		strokeShape.setRadius((*(*playerSelection)[i]).UnitShape.getRadius()+STROKE_SIZE);
		strokeShape.setPosition((*(*playerSelection)[i]).UnitShape.getPosition().x-STROKE_SIZE,(*(*playerSelection)[i]).UnitShape.getPosition().y-STROKE_SIZE);
		window.draw(strokeShape);
	}
}
//void pushBackSelectionToControlGroup(std::vector<Unit *>* playerSelection,std::vector<Unit>* playerUnits){//for control groups
//	for(unsigned int i=0;i<(*playerUnits).size();i++){
//		if((*playerUnits)[i].position.x-(*playerUnits)[i].UnitShape.getRadius()>=
//	}
//		
//}
int _tmain(int argc, _TCHAR* argv[])
{
	//Cheat protection:
	//Encrypt packets
	//or use Base64 and obsfuscate
	
	sf::ContextSettings settings;
	settings.antialiasingLevel=8;

	std::vector<Unit> playerUnits;
	std::vector<Unit> enemyUnits;
	std::vector<Unit *> playerSelection;

	for(unsigned int i=0;i<5;i++){
		playerUnits.push_back(Unit(Point2D((i+1)*150+50,668),sf::Color::Green,10));
	}
	for(unsigned int i=0;i<3;i++){
		enemyUnits.push_back(Unit(Point2D((i+1)*150+180,100),sf::Color::Red,20));
	}
	//Allows the player to queue up commands.
	//Non Shifted Commands empty the list.
	//Same with Unit Selection

	DecisionState mouseCommandState = Selecting;//True if valid command key is pressed.
	CommandEnum currentCommand;
	bool leftMouseClickedLastCycle = false;//For box selection
	

	selectionShape.setFillColor(sf::Color::Transparent);
	selectionShape.setOutlineThickness(3.0f);
	selectionShape.setOutlineColor(sf::Color::Black);
	selectionShape.setSize(sf::Vector2f(0.f,0.f));

	bool selectionDrawState = false; //user is selecting if true

	//Click -> Size Box -> Unclick -> Collision Detect Box with Player Units
	//if(shift){add pointers to those Units to Selection Vector}
	//else {empty Selection Vector, add those}
	//Select, Command Key, Left Click

	//Valid Commands: Attack, Move, Halt, Stop
	//LEFT CLICK DOES TWO THINGS. SELECTION AND COMMANDING
	//RIGHT CLICK ONLY DOES MOVE COMMAND
	bool inWindow =false;
	//std::string numberOfUnits="Number of Units Selected";
	//unsigned int counter=0;
	//numberOfUnits+std::to_string((static_cast<long long>( playerSelection.size())));
	//http://stackoverflow.com/questions/10664699/stdto-string-more-than-instance-of-overloaded-function-matches-the-argument/10666695#10666695
	//Oh wow a c++ static cast. Not my idea. VS2010 just doesn't fit the c++11 standard
	sf::RectangleShape background(sf::Vector2f(window.getSize().x,window.getSize().y));
	background.setFillColor(sf::Color::White);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
		//IF MOUSE IS ON THE WINDOW
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left)^leftMouseClickedLastCycle){//if Left Mouse Button state has changed since last cycle
			if(leftMouseClickedLastCycle==true){//if user has released left click button
				switch(mouseCommandState){
				case Selecting://Selection Change
					//pushBackSelection(&playerSelection,&playerUnits);
					grabOnScreenSelectedUnits(&playerUnits,&playerSelection);
					selectionDrawState=false;
					break;
				case Commanding:
					CommandSelectionUnits(currentCommand,&playerSelection);
					break;
				}
			}else{//if user has clicked left click button
				if(mouseIsOnScreen()){
					switch(mouseCommandState){
					case Selecting:
						selectionShape.setSize(sf::Vector2f(0,0));
						originalPoint=sf::Mouse::getPosition(window);
						updateSelection();
						selectionDrawState=true;
						break;
					case Commanding:
						CommandSelectionUnits(currentCommand,&playerSelection);
						break;
					}
				}
			}
			leftMouseClickedLastCycle^=true;//Swaps leftMouseClickLastCycle state
		}else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){//If Right mbutton clicked
			CommandSelectionUnits(Move,&playerSelection);
		}
		
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left)==true &&leftMouseClickedLastCycle==true && mouseCommandState==Selecting){//If the player is still holding down the left click button while selecting
				updateSelection();
		}
        window.clear();

		//printf("%s",selectionDrawState?"TRUE\n":"FALSE\n");
		window.draw(background);
		if(selectionDrawState==true){
			window.draw(selectionShape);
		}
		drawSelectionStroke(&playerSelection);
		drawUnitVector(&window,playerUnits);
		drawUnitVector(&window,enemyUnits);
        window.display();
    }

    return 0;
}
