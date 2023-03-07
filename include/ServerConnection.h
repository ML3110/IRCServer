#pragma once
#include "Connection.h"
#include "ConnectionInstance.h"

class ServerConnection :
	public Connection
{
public:
	ServerConnection(INet4Address* address);
	~ServerConnection();

	void BindSocket();
	void ListenOnSocket(int backlog);

	ConnectionInstance* AcceptClient();

	void StartServer(int backlog);
};

