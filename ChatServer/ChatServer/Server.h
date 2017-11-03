#pragma once
#include <iostream>
#include <map>
#include <functional>
#include <thread>

#include <SFML/Network/UdpSocket.hpp>

#include "Client.h"

struct Functions
{
	//void NewUser()
	//{
	//	std::cout << "det funkar" << std::endl;
	//}
};

class Server
{
public:
	Server();
	~Server();

	void Update();
	void Recieve(); // Thread
	void Send();

private:
	typedef void(Functions::*Func)(void); // Func is the name of this line
	typedef std::map<std::string, Func> func_map;
	typedef std::map<std::string, Client*> client_map;

	void CallFunction(const std::string& command, const std::string& input);
	void NewUser(const std::string name, const sf::IpAddress ip, const unsigned short port);

	func_map funcs;
	client_map clients;
	Functions func_struct;

	sf::UdpSocket socket;

	char buffer[1024]; // Message
};

