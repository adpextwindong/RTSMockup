#pragma once

#include <SFML/Graphics.hpp>

class fpsCounter
{
public:
	fpsCounter(void);
	fpsCounter(const sf::Font& font);
	~fpsCounter(void);
	void fpsCounter::draw(sf::RenderWindow* window);
	float getDelta(void);
	void updateFPSCounter(void);
	void getFPSText(void);
private:
	sf::Font theFont;
	sf::Clock fpsUpdateCounter;
	sf::Clock myClock;
	sf::Text fpsCounterText;
	long long fps;
};

