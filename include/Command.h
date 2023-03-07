#pragma once
#include "UserList.h"
#include <string>

class Command
{
	// Attributes
private:
	std::string serverName;
	std::string serverVersion;
	std::string MOTD;
	int userCount;
	int channelCount;

	// Constructors
public:
	Command();

	// Properties
public:
	void SetMOTD();
	void SetChannelCount(int count);
	void SetUserCount(int count);

	// Methods
public:
	std::string SendServerName();
	std::string SendServerVersion();
	std::string SendHelpMenu();
	std::string SendError(std::string input);
	std::string SendSuccess();
	std::string SendGoodbye();
	std::string SendUserJoin(std::string name);
	std::string SendOperator();
	std::string SendNotOperator();
	std::string SendMOTD();
	std::string SendWelcome(std::string name);
	std::string SendTerminated(std::string name);
};