#include "UserList.h"
#include <sstream>

UserList::UserList()
{

}

UserList::~UserList()
{

}

void UserList::AddUser(ConnectionInstance* user)
{
	this->userlist.push_back(user);
}

void UserList::Empty()
{
	this->userlist.clear();
}

void UserList::RemoveUser(std::string name)
{
	userlist.erase(std::remove_if(userlist.begin(), userlist.end(), [&name](ConnectionInstance* ci)
		{return name == ci->GetUsername(); }), userlist.end());
}

void UserList::BroadcastMessage(std::string strings)
{
	std::for_each(userlist.begin(), userlist.end(), [&strings](ConnectionInstance* ci) {
		send(ci->GetSocket(), strings.c_str(), strings.length(), 0);
		});
}

ConnectionInstance* UserList::GetUser(std::string username)
{
	auto it = std::find_if(userlist.begin(), userlist.end(), [&username](ConnectionInstance* ci) {
		return username == ci->GetUsername();
		});

	return userlist[std::distance(userlist.begin(), it)];
}

std::string UserList::Print(SOCKET sock, std::string channel, std::string name)
{
	std::string currentUsers;

	currentUsers += "=====================================\r\n";

	if (channel.empty())
	{
		currentUsers += "Current users on server:\r\n\=====================================\r\n";

		std::for_each(userlist.begin(), userlist.end(), [&sock, &channel, &currentUsers, &name](ConnectionInstance* ci) {
			currentUsers += ci->GetUsername().c_str();
			if (name == ci->GetUsername())
			{
				currentUsers += " <-- You";
			}
			currentUsers += "\r\n";
			});
	}

	else
	{
		currentUsers += "Current users in ";
		currentUsers += channel;
		currentUsers += ":\r\n=====================================\r\n";

		std::for_each(userlist.begin(), userlist.end(), [&sock, &channel, &currentUsers, &name](ConnectionInstance* ci) {
			if (ci->GetCurrentChannel() == channel)
			{
				currentUsers += ci->GetUsername().c_str();
				if (name == ci->GetUsername())
				{
					currentUsers += " <-- You";
				}
				currentUsers += "\r\n";
			}
			});
	}

	currentUsers += "=====================================\r\n";

	return currentUsers;
}

int UserList::GetCount()
{
	return this->userlist.size();
}

bool UserList::IsDuplicateNick(std::string name)
{
	auto it = std::find_if(userlist.begin(), userlist.end(), [&name](ConnectionInstance* ci) {
		return name == ci->GetUsername();
		});

	if (it != userlist.end())
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool UserList::UserExists(std::string name)
{
	auto it = std::find_if(userlist.begin(), userlist.end(), [&name](ConnectionInstance* ci) {
		return name == ci->GetUsername();
		});

	if (it != userlist.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}