#include "Client.h"


Client::Client()
{
	socket.bind(sf::Socket::AnyPort);

	host = host.getLocalAddress();
	host_port = 55002;

	Start();
}


Client::~Client()
{
}


void Client::Start()
{
	//std::cout << "Please enter the IP-address of the server..." << std::endl;
	std::string name;
	while (true)
	{
		std::cout << "Please enter your name..." << std::endl;
		std::getline(std::cin, name);

		if (name[0] == NULL)
		{
			std::cout << "Maybe write something for a name?" << std::endl;
			continue;
		}
		else if (name[0] == ' ')
		{
			std::cout << "Name can't begin with space" << std::endl;
			continue;
		}
		else if (name[name.size() - 1]  == ' ')
		{
			std::cout << "Name can't end with space" << std::endl;
			continue;
		}
		

		std::string message = "/n " + name;

		socket.send(message.c_str(), message.size() + 1, host, host_port);

		char buffer[1024];
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port;
		socket.receive(buffer, sizeof(buffer), received, sender, port);
		std::cout << sender.toString() << " said: " << std::string(buffer) << std::endl;

		if (buffer[0] != '/')
		{
			if (buffer[1] != 'r')
			{
				break;
			}
		}
	}
}


void Client::Update()
{
	std::thread recieve(&Client::Recieve, this);
	std::thread send(&Client::Send, this);

	recieve.join();
	send.join();

	while (true);
}



void Client::Recieve()
{
	while (true)
	{
		char buffer[1024];
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port;
		socket.receive(buffer, sizeof(buffer), received, sender, port);
		std::cout << sender.toString() << " said: " << std::string(buffer) << std::endl;
	}
}


//TODO:: DIS RIGHT HERE WAS DA PROBLEM
void Client::Send()
{
	while (true)
	{
		std::string message;
		std::getline(std::cin, message);

		if (message[0] == NULL)
		{
			std::cout << "fuck yo message\n";
			continue;
		}

		socket.send(message.c_str(), message.size() + 1, host, host_port);
	}
}