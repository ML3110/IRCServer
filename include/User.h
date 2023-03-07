#pragma once
#include <string>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

class User
{
	// Attributes
private:
	std::string name;
	sockaddr_in client;
	int clientSize = sizeof(client);
	SOCKET clientSocket;

	// Constructor
public:
	User(std::string name);
	User();
	~User();

	// Properties
public:
	std::string GetUser();
	SOCKET GetSocket();

	// Methods
public:
	void ChangeUserName(char newName);
	void RandomiseUserName();
	void CreateConnection(SOCKET listen);
};