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
	sf::IpAddress sender;
	unsigned short port;

	while (true)
	{
		socket.receive(buffer, sizeof(buffer), data_size, sender, port);

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

			for (int i = command_end + 1; i < data_size; i++) // For each character after command
			{
				input += buffer[i];
			}

			if (buffer[1] == 'n') // If command is n then it's a new user
				NewUser(input, sender, port);
			else // Otherwise it's a normal command
				CallFunction(command, input);
		}

		std::cout << sender.toString() << " said: " << buffer << std::endl;
	}
}


void Server::Send() //TODO:: This is NOT a thread
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
	}
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
}
