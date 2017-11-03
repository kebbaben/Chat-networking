#pragma once
#include <SFML/Network/UdpSocket.hpp>
#include <iostream>
#include <thread>

class Client
{
public:
	Client();
	~Client();

	void Update();
	void Recieve(); // Thread
	void Send();	// Thread

private:
	void Start();
	
	sf::UdpSocket socket;
	sf::IpAddress host;

	unsigned short host_port;
};

