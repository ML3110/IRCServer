#pragma once

//#include "Channel.h"
//#include "Message.h"
#include <WS2tcpip.h>
#include <string>
#include <time.h>
#include <stdio.h>
//#include "Command.h"

struct Message {
	char command[20];
	char content[512];
};

class ConnectionInstance
{
	// Attributes
private:
	SOCKET sock;
	std::string username;
	std::string channelName;
	Message message;
	char clientBuffer[512];
	char messageBuffer[512];
	char c;
	int count;
	bool commandSet;
	bool isOp;

	// Constructors
public:
	ConnectionInstance(SOCKET sock);
	~ConnectionInstance();

	// Properties
public:
	void SetSocket(SOCKET sock);
	SOCKET GetSocket();

	void SetUsername(std::string name);
	std::string GetUsername();

	void SetCurrentChannel(std::string channelName);
	std::string GetCurrentChannel();

	// Methods
public:
	bool ReceiveMessage();

	void ClearMessage();
	void ParseMessage();

	std::string GetMsg();

	std::string GetCmd();

	std::string GetContent();

	void AskUsername();
	bool IsCommand();
	bool IsOperator();
	void SetOperator();
};