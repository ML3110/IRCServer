#pragma once
#include "Connection.h"
#include "ConnectionInstance.h"
class ClientConnection :
	public Connection
{
public:
	ClientConnection(INet4Address* address);
	~ClientConnection();

	void BindConnection();
	ConnectionInstance* ConnectToServer();
};

