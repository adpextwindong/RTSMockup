#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
sf::RenderWindow window(sf::VideoMode(1024, 768), "RTS Mockup");

int main(void){

	sf::Texture tileSetTexture;
	if(!tileSetTexture.loadFromFile("mockupTileSet.png")){
		printf("Texture load failure");
		getchar();
		return -1;
	}

	sf::VertexArray tileSet;
	tileSet.setPrimitiveType(sf::Quads);
	tileSet.resize(4);

	sf::RenderStates tileSetStates;
	tileSetStates.blendMode=sf::BlendNone;
	tileSetStates.texture = &tileSetTexture;

	tileSet[0].position=sf::Vector2f(0,0);
	tileSet[1].position=sf::Vector2f(16,0);
	tileSet[2].position=sf::Vector2f(16,16);
	tileSet[3].position=sf::Vector2f(0,16);

	tileSet[0].texCoords=sf::Vector2f(0,0);
	tileSet[1].texCoords=sf::Vector2f(16,0);
	tileSet[2].texCoords=sf::Vector2f(16,16);
	tileSet[3].texCoords=sf::Vector2f(0,16);


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
        window.clear();

		window.draw(background);
		window.draw(tileSet,tileSetStates);
        window.display();
		
    }



}