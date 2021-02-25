#include "User.h"

User::User(std::string name)
{
	this->name = name;
}

User::User()
{

}

User::~User()
{

}

std::string User::GetUser()
{
	return this->name;
}

void User::ChangeUserName(char newName)
{
	this->name = newName;
}

void User::RandomiseUserName()
{
	this->name = "F";
}

void User::CreateConnection(SOCKET listen)
{
	this->clientSocket = accept(listen, (sockaddr*)&client, &clientSize);
}

SOCKET User::GetSocket()
{
	return this->clientSocket;
}