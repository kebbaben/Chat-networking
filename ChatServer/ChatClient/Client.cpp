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

		if (name[0] == NULL) // If nothing was written
		{
			std::cout << "Maybe write something for a name?" << std::endl;
			continue;
		}
		else if (name[0] == ' ') // If name starts with blankspace
		{
			std::cout << "Name can't begin with space" << std::endl;
			continue;
		}
		else if (name[name.size() - 1]  == ' ') // If name ends with blankspace
		{
			std::cout << "Name can't end with space" << std::endl;
			continue;
		}

		std::string message = "/n " + name; // Message = "command, name"
		socket.send(message.c_str(), message.size() + 1, host, host_port);

		char buffer[1024]; // Message
		std::size_t data_size = 0;
		sf::IpAddress sender;
		unsigned short port;

		socket.receive(buffer, sizeof(buffer), data_size, sender, port);
		std::cout << sender.toString() << " said: " << std::string(buffer) << std::endl;

		if (buffer[0] != '/') // If the message isn't a command
		{
			if (buffer[1] != 'r') // And if the command isn't r
			{
				break;
			}
		}
		// Proceed
	}
}


void Client::Update()
{
	// Creates two threads for sending and recieving messages
	std::thread recieve(&Client::Recieve, this);
	std::thread send(&Client::Send, this);

	recieve.join();
	send.join();

	// The function creating a thread must be running or the thread will die
	while (true);
}


void Client::Recieve()
{
	while (true)
	{
		char buffer[1024];
		std::size_t data_size = 0;
		sf::IpAddress sender;
		unsigned short port;

		socket.receive(buffer, sizeof(buffer), data_size, sender, port);
		std::cout << sender.toString() << " said: " << std::string(buffer) << std::endl;
	}
}


void Client::Send()
{
	while (true)
	{
		std::string message;
		std::getline(std::cin, message);

		if (message[0] == NULL) // Prevents clients from spamming to the server
		{
			std::cout << "fuck yo message\n";
			continue;
		}

		socket.send(message.c_str(), message.size() + 1, host, host_port);
	}
}