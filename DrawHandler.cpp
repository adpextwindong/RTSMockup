#include "StdAfx.h"
#include "DrawHandler.h"
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
fpsCounter theFPSCounter;
Tile tileSet;
Tile tileSetTexture;
Tile tileSetStates;

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



DrawHandler::DrawHandler(void)
{
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















