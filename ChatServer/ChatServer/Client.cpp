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
	return m_name;
}


const sf::IpAddress Client::GetIp() const
{
	return m_ip;
}


const unsigned short Client::GetPort() const
{
	return m_port;
}