#include "Server.h"


Server::Server()
{
	socket.bind(55002);
	//socket.setBlocking(false);

	//funcs.insert(std::make_pair("n", &Functions::NewUser));
}


Server::~Server()
{
}


void Server::Update()
{
	// Receive a message from anyone

	std::thread recieve(&Server::Recieve, this);
	recieve.join();

	//std::cout << client.back()->GetName() << " said: " << buffer << std::endl;

	// Send an answer
	//std::string message = "Welcome " + sender.toString();
	//socket.send(message.c_str(), message.size() + 1, sender, port);
}


void Server::Recieve()
{
	std::size_t received = 0;
	sf::IpAddress sender;
	unsigned short port;

	while (true)
	{
		socket.receive(buffer, sizeof(buffer), received, sender, port);

		if (buffer[0] == '/')
		{
			int command_end;
			std::string command;
			std::string input;

			for (int i = 1; i < received; i++)
			{
				if (buffer[i] == ' ')
				{
					command_end = i;
					break;
				}

				command += buffer[i];
			}
			for (int i = command_end + 1; i < received; i++)
			{
				input += buffer[i];
			}
			if (buffer[1] == 'n')
				NewUser(input, sender, port);
			else
				CallFunction(command, input);
		}

		std::cout << sender.toString() << " said: " << buffer << std::endl;
	}
}


void Server::Send()
{
	for (auto it = clients.begin(); it != clients.end(); it++)
	{
		socket.send(buffer, sizeof(buffer), it->second->GetIp(), it->second->GetPort());
	}
}


void Server::CallFunction(const std::string& command, const std::string& input)
{
	func_map::const_iterator iter;
	iter = funcs.find(command);
	//funcs[command]();
	if (iter != funcs.end())
	{
		(struct_func.*(iter->second))();
	}
}


void Server::NewUser(const std::string name, const sf::IpAddress ip, const unsigned short port)
{
	std::string message;

	client_map::const_iterator iter;
	iter = clients.find(name);

	if (iter == clients.end())
	{
		Client* client = new Client(name, ip, port);
		clients.insert(std::make_pair(name, client));
		message = "Welcome " + name + "!";
	}
	else
	{
		message = "/r Name is already taken. Please use another name...";
		
	}
	socket.send(message.c_str(), message.size() + 1, ip, port);
}
