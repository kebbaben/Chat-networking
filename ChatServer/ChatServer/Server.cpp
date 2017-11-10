#include "Server.h"


Server::Server()
{
	m_socket.bind(55001);
	//socket.setBlocking(false);

	// Inserts command functions
	m_funcs.insert(std::make_pair("n", &Functions::NewClient));
	m_funcs.insert(std::make_pair("w", &Functions::Whisper));
	m_funcs.insert(std::make_pair("disconnect", &Functions::Disconnect));
}


Server::~Server()
{
}


void Server::Update()
{
	// Creates a thread for recieving messages
	std::thread recieve(&Server::Recieve, this);
	recieve.join();
}


void Server::Recieve()
{
	// Data to be received
	char buffer[1024];
	std::size_t data_size = 0;
	sf::IpAddress senderIP;
	unsigned short senderPort;

	std::string senderName;
	std::string message;

	while (true)
	{
		// Resets noCommand
		m_isCommand = false;

		m_socket.receive(buffer, sizeof(buffer), data_size, senderIP, senderPort);

		message = buffer;

		// Detects username of sender, if it is registred
		for (auto it = m_clients.begin(); it != m_clients.end(); it++)
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

		// If possible command
		if (buffer[0] == '/')
		{
			size_t command_end = message.find(' ');
			std::string command;
			std::string input = "";
			bool hasInput = true;

			// If no space, indicating end of command, was found
			if (command_end == std::string::npos)
			{
				command_end = message.size();
				hasInput = false;
			}

			// Gets command from recieved message
			command = message.substr(1, command_end - 1);

			if (hasInput)
				input = message.substr(command_end + 1, message.size());

			CallFunction(senderName, senderIP, senderPort, input, command);
		}

		if (!m_isCommand)
		{
			std::string message = senderName + " said: " + std::string(buffer);

			// Sends message to all clients except the sender
			for (auto it = m_clients.begin(); it != m_clients.end(); it++)
			{
				if (it->first != senderName)
					Send(message, it->second->GetIp(), it->second->GetPort());
			}
		}

		std::cout << senderIP.toString() << " said: " << buffer << std::endl;
	}
}


const void Server::Send(const std::string message, const sf::IpAddress ip, const unsigned short port)
{
	m_socket.send(message.c_str(), message.size() + 1, ip, port);
}


void Server::CallFunction(const std::string name, const sf::IpAddress ip, const unsigned short port, const std::string& input, const std::string& command)
{
	// Find function with recieved command
	func_map::const_iterator iter = m_funcs.find(command);
	
	// If function is found
	if (iter != m_funcs.end()) 
	{
		// Calls function
		(m_funcStruct.*(iter->second))(name, ip, port, input, m_clients, m_socket);

		m_isCommand = true;
	}
	else
		m_isCommand = false;
}