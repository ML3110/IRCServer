#include "Connection.h"



Connection::Connection(INet4Address* address)
{
	this->address = address;
}


Connection::~Connection()
{
}

void Connection::InitialiseSocket()
{
	WSADATA wsaData;
	if (int errorCode = WSAStartup(MAKEWORD(2, 2), &wsaData) < 0)
	{
		throw WSAGetLastError();
	}
}

void Connection::CreateSocket()
{
	sockfd = socket(address->GetSockaddrRef()->sin_family, SOCK_STREAM, IPPROTO_TCP);

	if (sockfd <= 0)
	{
		throw WSAGetLastError();
	}
}