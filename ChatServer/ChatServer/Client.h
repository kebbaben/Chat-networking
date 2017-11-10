#pragma once
#include <string>

#include <SFML/Network/UdpSocket.hpp>


class Client
{
public:
	Client(std::string name, sf::IpAddress ip, unsigned short port);
	~Client();

	const std::string GetName() const;
	const sf::IpAddress GetIp() const;
	const unsigned short GetPort() const;

private:
	std::string m_name;
	sf::IpAddress m_ip;
	unsigned short m_port;
};

