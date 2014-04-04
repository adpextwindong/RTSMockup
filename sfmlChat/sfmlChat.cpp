#include "stdafx.h"
// sfmlChat.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "stdio.h"
#include <SFML\Network.hpp>
#define COMPILE_TIME_PORT 1337
void printError(char * errorStr){
	printf("\n**ERROR**\n%s",errorStr);
}
void networkCleanCString(char * str,unsigned int strlength){
	for(unsigned int i=strlength-1;i>0;i--){
		if(str[i]==-52){
			str[i]='\0';
		}else{
			break;
		}
	}
}
void clientMain(void){
	sf::TcpSocket serverSocket;
	sf::Socket::Status status = serverSocket.connect("127.0.0.1", COMPILE_TIME_PORT);
	if (status != sf::Socket::Done)
	{
		printError("Can't connect to Port");
		return;
	}else{
		printf("Connected. Dunzo\n");
	}
	printf("Awaiting string\n");
	char databuff[128];
	std::size_t recieved;
	if(serverSocket.receive(&databuff,128,recieved) != sf::Socket::Done){
		printError("Recieving Error");
	}
	//fix network cleaning shit
	networkCleanCString(databuff,128);
	
	//std::cout <<databuff << std::endl;
	printf("%s",databuff);
	//serverSocket.disconnect();
}
void serverMain(void){
	sf::TcpListener listener;
	if(listener.listen(COMPILE_TIME_PORT) != sf::Socket::Done){
		printError("Can't listen to Port");
		return;
	}

	sf::TcpSocket theClient;
	if (listener.accept(theClient) != sf::Socket::Done)
	{
		printError("Can't accept client");
		return;
	}else{
		printf("Connected. Dunzo\n");
	}
	char strbuff[128];
	for(;;){
		printf("Input information\n");
		scanf("%s",strbuff);
		if(theClient.send(&strbuff,strlen(strbuff)) != sf::Socket::Done){
			printError("Error Sending");
			return;
		}
		getchar();
	}
}


int menuChoice(void){
	int temp=0;
	printf("'1': Client or '2' Server\n");
	scanf("%d",&temp);
	printf("\n");
	return temp;
}
int _tmain(int argc, _TCHAR* argv[])
{
	for(;;){
		int choice = menuChoice();
		switch(choice){
		case 1:
			clientMain();
			break;
		case 2:
			serverMain();
			break;
		}
	}
	printf("Broke out");
	return 0;
}