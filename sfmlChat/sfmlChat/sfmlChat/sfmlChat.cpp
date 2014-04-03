// sfmlChat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include <SFML\Network.hpp>
#define COMPILE_TIME_PORT 1337

void clientMain(void){
	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect("127.0.0.1", COMPILE_TIME_PORT);
	if (status != sf::Socket::Done)
	{
		printf("\n**ERROR**\nCan't connect to Port %d",COMPILE_TIME_PORT);
		return;
	}else{
		printf("Connected. Dunzo");
	}
}

void serverMain(void){
	sf::TcpListener listener;
	if(listener.listen(COMPILE_TIME_PORT) != sf::Socket::Done){
		printf("\n**ERROR**\nCan't listen to Port %d",COMPILE_TIME_PORT);
		return;
	}
	sf::TcpSocket theClient;
	if (listener.accept(theClient) != sf::Socket::Done)
	{
		printf("\n**ERROR**\nCan't accept client");
		return;
	}else{
		printf("Connected. Dunzo");
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	int choice = 0;
	printf("'1': Client or '2' Server\n");
	

	printf("\n");

	switch(choice){
	case 1:
		clientMain();
		break;
	case 2:
		serverMain();
		break;
	case 0:
		return 0;
	}

	return 0;
}

