#pragma once
#include "ConnectionInstance.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

class UserList
{
	// Attributes
private:
	std::vector<ConnectionInstance*> userlist;

	// Constructor
public:
	UserList();
	~UserList();

	// Properties
public:
	void AddUser(ConnectionInstance* user);
	void RemoveUser(std::string name);
	int GetCount();
	ConnectionInstance* GetUser(std::string username);

	// Methods
public:
	void Empty();
	void BroadcastMessage(std::string ss);
	std::string Print(SOCKET sock, std::string channel, std::string name);
	bool IsDuplicateNick(std::string name);
	bool UserExists(std::string name);
};