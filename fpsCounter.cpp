#include "stdafx.h"
#include "fpsCounter.h"
#include <SFML/Graphics.hpp>
#include "Macros.h"

fpsCounter::fpsCounter(void)
{
}
fpsCounter::fpsCounter(const sf::Font& font){
	theFont = font;
	fpsCounterText.setFont(font);
	fpsCounterText.setPosition(1024*(7/8),768*(1/7));
	fpsCounterText.setColor(sf::Color::Red);
	fps=0;
	fpsUpdateCounter.restart().asSeconds();
}
float fpsCounter::getDelta()
{
	return myClock.restart().asSeconds()*100;
}
void fpsCounter::draw(sf::RenderWindow* window){
	(*window).draw(fpsCounterText);
}
void fpsCounter::updateFPSCounter(void){
	fps=(int)(1/myClock.restart().asSeconds());
	//printf("\n%d\n",fps);
	fpsCounterText.setString(std::to_string(fps));
	if(fpsUpdateCounter.getElapsedTime().asSeconds()>FPS_COUNTER_UPDATE_TIME){
		if(fps>60){
			fpsCounterText.setColor(sf::Color::Green);
		}else if(fps>30){
			fpsCounterText.setColor(sf::Color::Yellow);
		}else{
			fpsCounterText.setColor(sf::Color::Red);
		}
		fpsUpdateCounter.restart();
	}
}
fpsCounter::~fpsCounter(void)
{
}
