//TODO, MAKE CONFIG SYSTEM. FOR NOW THESE COMPILE TIME MACROS ARE FINE ENOUGH

#define PLAYER_COLOR sf::Color::Green
#define ENEMY_COLOR sf::Color::Red
#define SELECTION_STROKE_COLOR sf::Color::Blue
#define STROKE_SIZE 4
#define GAMEARRAYSIZE_MOCKUP 128
#define TILE_SIZE 32
#define FPS_COUNTER_UPDATE_TIME 0.5 //Seconds

enum Team{Green,Red};

sf::Color returnColor(Team param){
	switch(param){
	case Green:
		return sf::Color::Green;
	case Red:
		return sf::Color::Red;
	}
}