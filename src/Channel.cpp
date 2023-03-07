#include "Channel.h"

Channel::Channel()
{

}

Channel::Channel(std::string name)
{
	ul.Empty();
	this->MOTD.empty();
	this->topic.empty();
	this->name = name;
	this->channelCount = 0;
	this->userCount = 0;
}

Channel::~Channel()
{

}

std::string Channel::GetName()
{
	return this->name;
}

void Channel::SetTopic(std::string topic)
{
	this->topic = topic;
}

std::string Channel::GetTopic()
{
	return this->topic;
}

void Channel::SetMOTD()
{
	this->MOTD = "Welcome to the IRC server!\r\n";
	this->MOTD += "User count is: ";
	this->MOTD += std::to_string(this->userCount);
	this->MOTD += "\r\n";
	this->MOTD += "Channel count is: ";
	this->MOTD += std::to_string(this->channelCount);
	this->MOTD += "\r\n";
}

void Channel::SetUserCount(int count)
{
	this->userCount = count;
}

void Channel::SetChannelCount(int count)
{
	this->channelCount = count;
}

std::string Channel::GetMOTD()
{
	return this->MOTD;
}

void Channel::AddUser(ConnectionInstance* ci)
{
	this->ul.AddUser(ci);
}

void Channel::RemoveUser(std::string username)
{
	ul.RemoveUser(username);
}

void Channel::BroadcastMessage(std::string strings)
{
	ul.BroadcastMessage(strings);
}