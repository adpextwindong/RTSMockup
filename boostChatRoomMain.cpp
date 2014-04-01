#include "stdafx.h"//VS2010 Requirement
#include <boost\asio.hpp>

int _tmain(int argc, _TCHAR* argv[])
{
	int choice=-1;
	std::cout<<"Choose\nServer:'1'\nClient'2'\n";
	std::cin>>choice;
	std::cout<<std::endl;

	std::cout<<"Server~~~~~~~~"<<std::endl;
	std::cin>>choice;
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(),1337);
	boost::asio::ip::tcp::acceptor acceptor(io_service,endpoint);
	boost::asio::ip::tcp::socket mySocket(io_service);
	for(;;){
		acceptor.accept(mySocket);
		std::string theMessage("Gabe is a free loader");
		boost::system::error_code ignored_error;
		boost::asio::write(mySocket, boost::asio::buffer(theMessage),ignored_error);
		
	}
}