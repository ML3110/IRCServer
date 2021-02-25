#include "ServerConnection.h"



ServerConnection::ServerConnection(INet4Address* address) : Connection(address)
{
}


ServerConnection::~ServerConnection()
{
}

void ServerConnection::BindSocket()
{
	if (int errorCode = ::bind(sockfd, (sockaddr*)address->GetSockaddrRef(), (int)address->GetSocketAddressLengthVal()) < 0)
	{
		throw WSAGetLastError();
	}
}

void ServerConnection::ListenOnSocket(int backlog)
{
	if (listen(sockfd, backlog) < 0)
	{
		throw WSAGetLastError();
	}
}

ConnectionInstance* ServerConnection::AcceptClient()
{
	SOCKET clientSock;

	if ((clientSock = accept(sockfd, (sockaddr*)address->GetSockaddrRef(), (int*)address->GetSocketAddressLengthRef())) < 0)
	{
		throw WSAGetLastError();
	}

	return new ConnectionInstance(clientSock);
}

void ServerConnection::StartServer(int backlog)
{
	InitialiseSocket();
	CreateSocket();
	BindSocket();
	ListenOnSocket(backlog);
}