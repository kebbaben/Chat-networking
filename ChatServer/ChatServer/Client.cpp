#include "Client.h"



Client::Client(std::string name, sf::IpAddress ip, unsigned short port)
{
	m_name = name;
	m_ip = ip;
	m_port = port;
}


Client::~Client()
{
}


const std::string Client::GetName() const
{
	return std::string();
}


const sf::IpAddress Client::GetIp() const
{
	return sf::IpAddress();
}


const unsigned short Client::GetPort() const
{
	return 0;
}
