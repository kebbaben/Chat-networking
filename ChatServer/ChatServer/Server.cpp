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
	// Creates a thread for recieving messages
	std::thread recieve(&Server::Recieve, this);
	recieve.join();

	// The function creating a thread must be running or the thread will die
	while (true);
}


void Server::Recieve()
{
	std::size_t data_size = 0; // Amount of characters in buffer
	sf::IpAddress senderIP;
	unsigned short senderPort;
	std::string senderName;

	while (true)
	{
		// Resets noCommand
		noCommand = true;

		socket.receive(buffer, sizeof(buffer), data_size, senderIP, senderPort);

		// Detects username of sender
		for (auto it = clients.begin(); it != clients.end(); it++)
		{
			if (senderIP == it->second->GetIp() && senderPort == it->second->GetPort())
				senderName = it->second->GetName();
		}

		if (buffer[0] == '/') // If command
		{
			int command_end;
			std::string command;
			std::string input;

			for (int i = 1; i < data_size; i++) // For each command character
			{
				if (buffer[i] == ' ') // If end of command
				{
					command_end = i;
					break;
				}

				command += buffer[i];
			}

			for (int i = command_end + 1; i < data_size - 1; i++) // For each character after command
			{
				input += buffer[i];
			}

			if (buffer[1] == 'n') // If command is n then it's a new user
				NewUser(input, senderIP, senderPort);
			else // Otherwise it's a normal command
				CallFunction(command, input);
		}

		if (noCommand)
		{
			std::string message = senderName + " said: " + std::string(buffer); // NULL after name

			// Sends message to all clients except the sender
			for (auto it = clients.begin(); it != clients.end(); it++)
			{
				if (it->first != senderName)
					socket.send(message.c_str(), message.size() + 1, it->second->GetIp(), it->second->GetPort());
			}
		}

		std::cout << senderIP.toString() << " said: " << buffer << std::endl;
	}
}


void Server::Send() //TODO:: This is NOT a thread, should it be?
{
	for (auto it = clients.begin(); it != clients.end(); it++)
	{
		socket.send(buffer, sizeof(buffer), it->second->GetIp(), it->second->GetPort());
	}
}


void Server::CallFunction(const std::string& command, const std::string& input)
{
	func_map::const_iterator iter;
	iter = funcs.find(command); // Find function with recieved command
	//funcs[command]();
	if (iter != funcs.end()) // If function is found
	{
		(func_struct.*(iter->second))(); // Calls function
		noCommand = false;
	}
	else
		noCommand = true;
}


void Server::NewUser(const std::string name, const sf::IpAddress ip, const unsigned short port)
{
	std::string message;

	client_map::const_iterator iter;
	iter = clients.find(name); // Look for a client with the same name

	if (iter == clients.end()) // If name isn't already taken
	{
		// Creates new client anda adds it to the map
		Client* client = new Client(name, ip, port);
		clients.insert(std::make_pair(name, client));
		message = "Welcome " + name + "!";
	}
	else // If name already is taken
	{
		message = "/r Name is already taken. Please use another name...";
	}

	socket.send(message.c_str(), message.size() + 1, ip, port);

	noCommand = false;
}
