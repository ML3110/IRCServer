#define _CRT_SECURE_NO_WARNINGS // i know this isn't ideal, but it's only used for the time. Not a neccessity, but I added it because it looks better IMO

#include "ConnectionInstance.h"
//#include "Message.h"

ConnectionInstance::ConnectionInstance(SOCKET sock)
{
	this->sock = sock;
	this->commandSet = false;
	this->isOp = false;
}

ConnectionInstance::~ConnectionInstance()
{
}

void ConnectionInstance::SetSocket(SOCKET sock)
{
	this->sock = sock;
}

SOCKET ConnectionInstance::GetSocket()
{
	return sock;
}

void ConnectionInstance::SetUsername(std::string name)
{
	this->username = name;
}

std::string ConnectionInstance::GetUsername()
{
	return this->username;
}

void ConnectionInstance::SetCurrentChannel(std::string channelName)
{
	this->channelName = channelName;
}

std::string ConnectionInstance::GetCurrentChannel()
{
	return this->channelName;
}

void ConnectionInstance::AskUsername()
{
	char namebuff[50];
	std::string message;

	message += "Please enter your username (4 - 9 characters): ";

	while (strlen(namebuff) > 9 || strlen(namebuff) < 4)
	{
		if (namebuff[0] != '\r')
		{
			// request username
			send(this->sock, message.c_str(), message.length(), 0);

			memset(namebuff, 0, sizeof(namebuff));
		}
		// receive from client
		recv(this->sock, namebuff, sizeof(namebuff), 0);
	}

	// convert to lowercase
	_strlwr_s(namebuff);

	// set username
	SetUsername(namebuff);
}

bool ConnectionInstance::ReceiveMessage()
{
	// set each receive
	bool rcvd = false;
	this->commandSet = false;
	int nread;

	nread = 0;

	ClearMessage();

	this->count = 0;
	this->c = ' ';

	while (!rcvd)
	{
		nread = recv(sock, this->clientBuffer, sizeof(this->clientBuffer), 0);
		if (nread == 0)
		{
			return false;
			break;
		}
		if (clientBuffer[0] != '\r')
		{
			rcvd = true;
		}
	}

	return true;
}

void ConnectionInstance::ParseMessage()
{
	this->c = this->clientBuffer[this->count];

	// if begins with '/' we're receiving a command
	if (this->c == '/' && this->count == 0)
	{
		// set old count
		int oldCount = 0;

		// while not finished reading command
		while (this->c != NULL && this->c != '\r')
		{
			// convert to lower
			this->c = tolower(c);

			// parse
			this->message.command[this->count] = c;
			this->count++;
			this->c = this->clientBuffer[this->count];

			// if its a space, we've finished command so read content
			if (this->c == ' ')
			{
				oldCount = this->count;

				this->count = 0;

				// while not finished reading content
				while (this->c != NULL && this->c != '\r')
				{
					while (this->c == ' ')
					{
						oldCount++;
						this->c = this->clientBuffer[this->count];
					}

					this->c = this->clientBuffer[oldCount];
					this->c = tolower(this->c);

					this->message.content[this->count] = this->c;
					this->count++;
					oldCount++;
				}
			}
			this->commandSet = true;
		}
	}

	// else we're just reading content
	else
	{
		this->count = 0;

		while (this->c != NULL && this->c != '\r')
		{
			this->message.content[this->count] = this->c;
			this->count++;
			this->c = this->clientBuffer[this->count];
		}
	}
}

std::string ConnectionInstance::GetMsg()
{
	time_t rawtime;

	struct tm* timeinfo;

	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%T", timeinfo);

	std::string s;

	s += buffer;

	s += " ";

	s += this->username;
	s += "@";
	s += this->channelName;
	s += ": ";
	s += this->message.content;
	s += "\r\n";

	return s;
}

std::string ConnectionInstance::GetContent()
{
	std::string s;

	s += this->message.content;

	return s;
}

void ConnectionInstance::ClearMessage()
{
	memset(this->message.command, 0, sizeof(this->message.command));
	memset(this->message.content, 0, sizeof(this->message.content));
	memset(this->messageBuffer, 0, sizeof(this->messageBuffer));
	memset(this->clientBuffer, 0, sizeof(this->clientBuffer));
}

std::string ConnectionInstance::GetCmd()
{
	return this->message.command;
}

bool ConnectionInstance::IsCommand()
{
	return this->commandSet;
}

bool ConnectionInstance::IsOperator()
{
	return this->isOp;
}

void ConnectionInstance::SetOperator()
{
	this->isOp = true;
}