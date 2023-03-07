#include "INet4Address.h"



INet4Address::INet4Address()
{
}

INet4Address::INet4Address(char* add, int port)
{
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET, add, &address.sin_addr);
	length = sizeof(address);
}

INet4Address::INet4Address(int port)
{
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	length = sizeof(address);
}


INet4Address::~INet4Address()
{
}

sockaddr_in* INet4Address::GetSockaddrRef()
{
	//Pointer to structure
	return &address;
}

size_t* INet4Address::GetSocketAddressLengthRef()
{
	//Pointer to length of structure
	return &length;
}

size_t INet4Address::GetSocketAddressLengthVal()
{
	//Value length of structure
	return length;
}

void INet4Address::GetSocketAddress(char* cstr, int len)
{
	//Address as a string - note, uses the cstr parameter
	inet_ntop(AF_INET, &address.sin_addr, cstr, len);
}

std::string INet4Address::GetSocketAddress()
{
	char tString[INET_ADDRSTRLEN];

	GetSocketAddress(tString, INET_ADDRSTRLEN);
	return string(tString);
}

int INet4Address::GetPort()
{
	//Return the port.  Remember, convert to host byte order
	return ntohs(address.sin_port);
}

void INet4Address::Print()
{
	//For debugging.
	char tString[INET_ADDRSTRLEN];

	GetSocketAddress(tString, INET_ADDRSTRLEN);

	cout << "INet4Address: " << tString << ":" << ntohs(address.sin_port) << endl;
}