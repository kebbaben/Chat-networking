#include <thread>

#include "Client.h"


Client::Client()
{
	//atexit([]() { Client::Instance.Disconnect(); });

	m_socket.bind(sf::Socket::AnyPort);

	//host_ip = host_ip.getLocalAddress();
	m_host_port = 55001;

	connected = false;

	Connect();
}


Client::~Client()
{
}


void Client::Connect()
{
	std::string ipAddress;

	while (true)
	{
		std::cout << "Please enter the IP-address of the server..." << std::endl;
		std::getline(std::cin, ipAddress);

		// If nothing was written
		if (ipAddress.size() <= 0) 
		{
			std::cout << "You didn't type anything" << std::endl;
		}
		else
		{
			m_host_ip = ipAddress;
			break;
		}
	}
	
	std::string name;

	while (true)
	{
		std::cout << "Please enter your name..." << std::endl;
		std::getline(std::cin, name);

		// If nothing was written
		if (name.size() <= 0) 
		{
			std::cout << "You didn't type anything" << std::endl;
			continue;
		}
		// If name starts or ends with space with blankspace
		else if (name[0] == ' ' || name[name.size() - 1] == ' ')
		{
			std::cout << "Name can't begin or end with space" << std::endl;
			continue;
		}

		std::string message = "/n " + name;
		m_socket.send(message.c_str(), message.size() + 1, m_host_ip, m_host_port);

		// Data to be received
		char buffer[1024];
		std::size_t data_size = 0;
		sf::IpAddress sender;
		unsigned short port;

		m_socket.receive(buffer, sizeof(buffer), data_size, sender, port);

		// Prints received message
		std::cout << buffer << std::endl;

		// If the requested name isn't already taken
		if (buffer[0] != '/') 
		{
			connected = true;
			break;
		}
	}
}


void Client::Update()
{
	// Creates threads for sending and recieving messages
	std::thread recieve(&Client::Recieve, this);
	std::thread send(&Client::Send, this);

	recieve.join();
	send.join();
}


void Client::Recieve()
{
	while (true)
	{
		if (connected)
		{
			// Data to be received
			char buffer[1024];
			std::size_t data_size = 0;
			sf::IpAddress sender;
			unsigned short port;

			m_socket.receive(buffer, sizeof(buffer), data_size, sender, port);

			// Prints received message
			std::cout << buffer << std::endl;
		}
	}
}


void Client::Send()
{
	while (true)
	{
		std::string message;
		std::getline(std::cin, message);

		// Prevents clients from sending nothing to the server
		if (message.size() <= 0) 
		{
			std::cout << "You didn't type anything" << std::endl;
			continue;
		}

		m_socket.send(message.c_str(), message.size() + 1, m_host_ip, m_host_port);

		if (message == "/disconnect")
		{
			m_host_ip = m_host_ip.None;
			connected = false;
		}

		if (message == "/connect" && !connected)
		{
			Connect();
		}
	}
}