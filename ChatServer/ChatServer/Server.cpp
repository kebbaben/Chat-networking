#include "Server.h"


Server::Server()
{
	socket.bind(55002);
	//socket.setBlocking(false);

	funcs.insert(std::make_pair("n", &Functions::NewUser));
	funcs.insert(std::make_pair("w", &Functions::Whisper));
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
	while (true)
	{
		//atexit(Recieve)
	}
}


void Server::Recieve()
{
	std::size_t data_size = 0;
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
			{
				senderName = it->second->GetName();
				break;
			}
			else
			{
				senderName = "";
			}
		}

		if (buffer[0] == '/') // If command
		{
			int command_end;
			std::string command;
			std::string input;

			for (int i = 1; i < data_size; i++) // For each command character
			{
				if (buffer[i] == ' ' || buffer[i] == NULL) // If end of command
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

			CallFunction(senderName, senderIP, senderPort, input, command);
		}

		if (noCommand)
		{
			std::string message = senderName + " said: " + std::string(buffer);

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


void Server::CallFunction(const std::string name, const sf::IpAddress ip, const unsigned short port, const std::string& input, const std::string& command)
{
	func_map::const_iterator iter;
	iter = funcs.find(command); // Find function with recieved command
	//funcs[command]();
	if (iter != funcs.end()) // If function is found
	{
		(func_struct.*(iter->second))(name, ip, port, input, clients, socket); // Calls function
		noCommand = false;
	}
	else
		noCommand = true;
}


void Server::NewUser(const std::string name, const sf::IpAddress ip, const unsigned short port)
{
	std::string message;

	if (!FindClient(name)) // If name isn't already taken
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


const bool Server::FindClient(const std::string name) const
{
	client_map::const_iterator iter;

	// Look for a client with the given name
	iter = clients.find(name); 

	// If client wasn't found
	if (iter == clients.end())
		return false;
	else
		return true;
}