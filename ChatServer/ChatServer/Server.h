#pragma once
#include <iostream>
#include <map>
#include <functional>
#include <thread>

#include <SFML/Network/UdpSocket.hpp>

#include "Client.h"

struct Functions
{
	typedef std::map<std::string, Client*> client_map;


	// Sends a private message to another client
	void Whisper(const std::string senderName, const sf::IpAddress senderIP, const unsigned short senderPort, const std::string input, client_map& clients, sf::UdpSocket& socket)
	{
		std::string targetName;
		std::string message;
		int nameEnd;

		// If use of command is incorrect
		if (input[0] != '<' || input.find('>') == std::string::npos)
		{
			message = "False use of command. Correct usage is: /w <target username>...";
			socket.send(message.c_str(), message.size() + 1, senderIP, senderPort);
			return;
		}

		// Goes through every character
		for (int i = 1; i < input.length(); i++)
		{
			// If end of name is detected
			if (input[i] == '>')
			{
				client_map::const_iterator iter = clients.find(targetName);

				// If client with requested username doesn't exist
				if (iter == clients.end())
				{
					message = "No connected user has the name '" + targetName + "'";
					socket.send(message.c_str(), message.size() + 1, senderIP, senderPort);
					return;
				}

				nameEnd = i;
				break;
			}

			// Adds character to name
			targetName += input[i];
		}

		message = senderName + " whispered: " + input.substr(nameEnd + 1, input.length());

		socket.send(message.c_str(), message.size() + 1, clients[targetName]->GetIp(), clients[targetName]->GetPort());
	}


	// Registers a new user
	void NewUser(const std::string name, const sf::IpAddress ip, const unsigned short port, const std::string input, client_map& clients, sf::UdpSocket& socket)
	{
		// If user isn't registered
		if (!FindClient(name, clients))
		{
			std::string message;

			if (!FindClient(input, clients)) // If name isn't already taken
			{
				// Creates new client anda adds it to the map
				Client* client = new Client(input, ip, port);
				clients.insert(std::make_pair(input, client));
				message = "Welcome " + input + "!";
			}
			else // If name already is taken
			{
				message = "/r Name is already taken. Please use another name...";
			}

			socket.send(message.c_str(), message.size() + 1, ip, port);
		}
	}


	// Finds out if a user with a certain name exists
	bool FindClient(const std::string name, const client_map& clients)
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
};

class Server
{
public:
	Server();
	~Server();

	void Update();
	void Recieve();
	void Send();

private:
	typedef std::map<std::string, Client*> client_map;
	typedef void(Functions::*Func)(const std::string senderName, const sf::IpAddress senderIP, const unsigned short senderPort, const std::string input, client_map& clients, sf::UdpSocket& socket); // Func is the name of this line
	typedef std::map<std::string, Func> func_map;

	void CallFunction(const std::string name, const sf::IpAddress ip, const unsigned short port, const std::string& input, const std::string& command);
	void NewUser(const std::string name, const sf::IpAddress ip, const unsigned short port);
	const bool FindClient(const std::string) const;

	func_map funcs;
	client_map clients;
	Functions func_struct;

	sf::UdpSocket socket;

	char buffer[1024]; // Message

	bool noCommand;
};

