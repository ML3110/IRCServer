#pragma once

//#include <winsock.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>

using namespace std;

class INet4Address
{
public:
	INet4Address();
	INet4Address(char* add, int port);
	INet4Address(int port);

	~INet4Address();

	//To get a sockaddr_in pointer to pass to connect
	sockaddr_in* GetSockaddrRef();

	//The two types of length, used depending on how we need to pass them
	size_t* GetSocketAddressLengthRef();
	size_t GetSocketAddressLengthVal();

	//We may want to print the address and port
	//pass in string length so we don't overfill the buffer
	void GetSocketAddress(char*, int len);
	std::string GetSocketAddress();

	int GetPort();

	//Debugging function
	void Print();

private:
	sockaddr_in address; //Address structure
	size_t length; //Object size
};

