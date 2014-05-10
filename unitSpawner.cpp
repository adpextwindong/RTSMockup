#include "StdAfx.h"
#include "unitSpawner.h"
#include "Macros.h"
#include "Math.h"

	unitSpawner::unitSpawner(std::vector<Unit> * _teamUnitList,Unit _unitTemplate,sf::Vector2i _position,sf::Texture * _spawnerText):
	teamUnitList(_teamUnitList),
	unitTemplate(_unitTemplate),
	position(_position)
{
	spawnerSprite.setTexture(*_spawnerText);
	spawnerSprite.setPosition(position);
}

void unitSpawner::draw(sf::RenderWindow * win){
	win->draw(spawnerSprite);
}
int giveRand(unsigned int absRange){
	int num = std::rand()%(absRange+1);
	int sign = std::rand()%2 == 0 ? -1 : 1;
	return sign * num;
}
void unitSpawner::spawnUnit(unsigned int num,unsigned spreadRange){
	for(unsigned int i = 0; i<num;i++){
		Unit tempUnit(unitTemplate);
		tempUnit.UnitShape.setPosition(position);
		sf::Vector2i tempPos = sf::Vector2i(position);
		tempPos.x += giveRand(spreadRange);
		tempPos.y += giveRand(spreadRange);
		tempUnit.unitCommands.push_back(Command(Move,tempPos,nullptr));
		teamUnitList->push_back(tempUnit);
	}
}

unitSpawner::~unitSpawner(void)
{
}
