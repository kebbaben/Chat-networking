#pragma once
#include <iostream>

#include <SFML/Network/UdpSocket.hpp>


class Client
{
public:
	Client();
	~Client();

	void Update();

private:
	void Connect();
	void Recieve();			// Thread
	void Send();			// Thread
	
	sf::UdpSocket m_socket;
	sf::IpAddress m_host_ip;
	unsigned short m_host_port;
	bool m_connected;
};