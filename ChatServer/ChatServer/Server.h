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
	void Recieve();
	void Send();

private:
	typedef void(Functions::*Func)(void);
	typedef std::map<std::string, Func> func_map;
	typedef std::map<std::string, Client*> client_map;

	void CallFunction(const std::string& command, const std::string& input);
	void NewUser(const std::string name, const sf::IpAddress ip, const unsigned short port);

	func_map funcs;
	client_map clients;
	Functions struct_func;

	sf::UdpSocket socket;

	char buffer[1024];
};

