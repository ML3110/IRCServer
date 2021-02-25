#pragma once
#include "UserList.h"

class Channel
{
	// Attributes
private:
	std::string MOTD;
	std::string name; // check the naming convention
	std::string topic;
	int userCount;
	int channelCount;
	UserList ul;

	// Constructors
public:
	Channel();
	Channel(std::string name);
	~Channel();

	// Properties
public:
	std::string GetName();
	void SetTopic(std::string topic);
	std::string GetTopic();
	void SetMOTD();
	std::string GetMOTD();
	void SetUserCount(int count);
	void SetChannelCount(int count);

	// Methods
public:
	void AddUser(ConnectionInstance* ci);
	void RemoveUser(std::string username);
	void BroadcastMessage(std::string strings);
	void WelcomeUser(ConnectionInstance* ci);
};