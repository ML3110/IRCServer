#include "Command.h"

Command::Command()
{
	this->serverName = "NetworkedApps IRC";
	this->serverVersion = "0.1";
}

std::string Command::SendHelpMenu()
{
	std::string s;

	s += "=====================================\r\nCommand menu:\r\n=====================================\r\n";
	s += "/help\t\t\tCommand menu\r\n";
	s += "/nick <nickname>\tChange name\r\n";
	s += "/users <channel>\tDisplay active users in a specific channel\r\n\t\t\tOr leave blank for all\r\n";
	s += "/channels\t\tDisplay available channels\r\n";
	s += "/join <channel>\t\tJoins specified channel\r\n";
	s += "/make <channel>\t\tCreate a new channel\r\n";
	s += "/kill <user>\t\tKill specified user connection\r\n";
	s += "/quit\t\t\tExit IRC server\r\n";
	s += "=====================================\r\n";

	return s;
}

std::string Command::SendError(std::string input)
{
	std::string s;

	s += "Error on input: '";
	s += input;
	s += "'. Unable to execute command. Please check entered content.\r\n";

	return s;
}

std::string Command::SendSuccess()
{
	std::string s;

	s += "Success!\r\n";

	return s;
}

std::string Command::SendGoodbye()
{
	std::string s;

	s += "Goodbye.\r\n";

	return s;
}

std::string Command::SendUserJoin(std::string name)
{
	std::string s;

	s += name;
	s += " has joined the channel!\r\n";

	return s;
}

std::string Command::SendOperator()
{
	std::string s;

	s += "You are now an operator.\r\n";

	return s;
}

std::string Command::SendNotOperator()
{
	std::string s;

	s += "Sorry, you are not an operator.\r\n";

	return s;
}

std::string Command::SendServerName()
{
	std::string s;

	s += "Server name: ";
	s += serverName;
	s += "\r\n";

	return s;
}

std::string Command::SendServerVersion()
{
	std::string s;

	s += "Server version: ";
	s += serverVersion;
	s += "\r\n";

	return s;
}

void Command::SetChannelCount(int count)
{
	this->channelCount = count;
}

void Command::SetUserCount(int count)
{
	this->userCount = count;
}

std::string Command::SendMOTD()
{
	std::string s;

	s = "=====================================\r\n";
	s += "=     Welcome to the IRC server!    =\r\n";
	s += "=====================================\r\n";
	s += "User count is: ";
	s += std::to_string(this->userCount);

	s += "\r\n";
	s += "Channel count is: ";
	s += std::to_string(this->channelCount);

	s += "\r\n=====================================\r\n";
	s += SendServerName();
	s += SendServerVersion();
	s += "=====================================\r\n";

	return s;
}

std::string Command::SendWelcome(std::string name)
{
	std::string s;

	s += "=====================================\r\n";
	s += "Hello ";
	s += name;
	s += ". Type /help for available commands.";
	s += "\r\n=====================================\r\n\r\n";

	return s;
}

std::string Command::SendTerminated(std::string name)
{
	std::string s;

	s += name;
	s += " has been terminated.";
	s += "\r\n";

	return s;
}
