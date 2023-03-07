#include "ServerConnection.h"
#include "ConnectionInstance.h"
#include <iomanip>
#include <thread>
#include "INet4Address.h"
#include "User.h"
#include "ChannelList.h"
#include "Channel.h"
#include "UserList.h"
#include "Command.h"
#include <sstream>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

// TODO:
// 0.  Messages per channel/room			-done-
// 1.  Print out commands					-done-
// 2.  Error checking RFC1459 6/6.1
//     - Channel exists - duplicate			-done-
//     - Channel exists - join invalid		-done-
//	   - User exists (kill)					-done-
// 3.  Delete user when quit/exit			-done-
//     - re-check user list					-done-
// 5.  Admin only commands?
//	   - Kill RFC1459 4.6.1					-done-
//	   - Delete room
// 6.  User quit							-done-
// 7.  MOTD
//     - Current user count					-done-
//     - Current server count				-done-
//     - Server name and version			-done-
// 8.  Private messages				- Maybe. Will need to change message to include parameter as
//									along with the command /private, will need to include username
//                                  then the actual message content
// 9.  Clean up user session if bytesrecvd = 0	-done-
// 10. Print current room/channel				-done-
// 11. User name change							-done-
// 12. Make the command headings and titles tidier
//	   - help/command menu						-done-
//     - users list								-done-
//	   - channels list							-done-
//     - MOTD									-done-
//     - channel topic
// 13. Channels - on join, needs to
//	   reply with channel topic and user list
//     only of channel RFC1459 4.2.1
// 14. Kick user from channels RFC1459 4.2.8
// 15. Ping/Pong RFC1459 4.6.2
// 16. Errors RFC1459 6 / 6.1
// 17. Flood prevention RFC1459 8.10
// 18. Limit username							-done-

// Bugs
// 1. Users can join with duplicate usernames. This causes problems with sessions and killing connections

void ClientHandler(ConnectionInstance* ci, UserList* ul, ChannelList* cl, Command* server)
{
	server->SetChannelCount(cl->GetCount());
	server->SetUserCount(ul->GetCount());

	// add user to user list for main
	ul->AddUser(ci);

	// send motd
	send(ci->GetSocket(),
		server->SendMOTD().c_str(),
		server->SendMOTD().length(),
		0);

	// ask username
	ci->AskUsername();

	// add user to main channel
	cl->GetChannel("main")->AddUser(ci);

	// set user channel to main
	ci->SetCurrentChannel("main");

	// alert that user has joined the chat
	std::cout << "User joined chat: " << ci->GetUsername() << std::endl;

	// send welcome
	send(ci->GetSocket(),
		server->SendWelcome(ci->GetUsername()).c_str(),
		server->SendWelcome(ci->GetUsername()).length(),
		0);

	// broadcast user join message
	cl->GetChannel(ci->GetCurrentChannel())->BroadcastMessage(server->SendUserJoin(ci->GetUsername()));

	while (true) {

		// if received message, parse it
		if (ci->ReceiveMessage())
		{
			ci->ParseMessage();
		}

		// otherwise use has disconnx so break and kill
		else
		{
			break;
		}

		// if user has set command
		if (ci->IsCommand())
		{

			// if help, send help menu
			if (strcmp(ci->GetCmd().c_str(), "/help") == 0)
			{
				send(ci->GetSocket(),
					server->SendHelpMenu().c_str(),
					server->SendHelpMenu().length(),
					0);
			}

			// if users, send list of users the user has requested for which channel
			else if (strcmp(ci->GetCmd().c_str(), "/users") == 0)
			{
				if (cl->ChannelExists(ci->GetContent()))
				{
					send(ci->GetSocket(),
						ul->Print(ci->GetSocket(), ci->GetContent(), ci->GetUsername()).c_str(),
						ul->Print(ci->GetSocket(), ci->GetContent(), ci->GetUsername()).length(),
						0);
				}
				// if they haven't specified a channel, or channel invalid, print users on whole server
				else
				{
					send(ci->GetSocket(),
						ul->Print(ci->GetSocket(), "", ci->GetUsername()).c_str(),
						ul->Print(ci->GetSocket(), "", ci->GetUsername()).length(),
						0);
				}
			}

			// if make, create the channel
			else if (strcmp(ci->GetCmd().c_str(), "/make") == 0)
			{
				// if the channel already exists, send error
				if (cl->ChannelExists(ci->GetContent()))
				{
					send(ci->GetSocket(),
						server->SendError(ci->GetContent()).c_str(),
						server->SendError(ci->GetContent()).length(),
						0);
				}
				// else make the channel and send success
				else
				{
					Channel* channel = new Channel(ci->GetContent());
					cl->AddChannel(channel);
					send(ci->GetSocket(),
						server->SendSuccess().c_str(),
						server->SendSuccess().length(),
						0);
				}
			}

			// if channels, print list of channels
			else if (strcmp(ci->GetCmd().c_str(), "/channels") == 0)
			{
				send(ci->GetSocket(),
					cl->Print(ci->GetCurrentChannel()).c_str(),
					cl->Print(ci->GetCurrentChannel()).length(),
					0);
			}

			// if join, join the specified channel
			else if (strcmp(ci->GetCmd().c_str(), "/join") == 0)
			{
				// if it exists, remove from current, set current to new and broadcast that user has joined channel
				if (cl->ChannelExists(ci->GetContent()))
				{
					cl->GetChannel(ci->GetCurrentChannel())->RemoveUser(ci->GetUsername());
					cl->GetChannel(ci->GetContent())->AddUser(ci);
					ci->SetCurrentChannel(ci->GetContent());
					send(ci->GetSocket(),
						cl->GetChannel(ci->GetCurrentChannel())->GetTopic().c_str(),
						cl->GetChannel(ci->GetCurrentChannel())->GetTopic().length(),
						0);

					cl->GetChannel(ci->GetCurrentChannel())->BroadcastMessage(server->SendUserJoin(ci->GetUsername()).c_str());
				}
				// else send error that channel probably doesn't exist
				else
				{
					send(ci->GetSocket(),
						server->SendError(ci->GetContent()).c_str(),
						server->SendError(ci->GetContent()).length(),
						0);
				}
			}

			// if nick, change nickname
			else if (strcmp(ci->GetCmd().c_str(), "/nick") == 0)
			{
				// if user doesn't exist, change and send success
				if (!ul->UserExists(ci->GetContent()))
				{
					if (ci->GetContent().length() > 3 && ci->GetContent().length() < 9)
					{
						ci->SetUsername(ci->GetContent());
						send(ci->GetSocket(),
							server->SendSuccess().c_str(),
							server->SendSuccess().length(),
							0);
					}
				}
				// if user already exists, send error
				else
				{
					send(ci->GetSocket(),
						server->SendError(ci->GetContent()).c_str(),
						server->SendError(ci->GetContent()).length(),
						0);
				}
			}

			// if kill, close the socket on the specified user
			else if (strcmp(ci->GetCmd().c_str(), "/kill") == 0)
			{
				// if the user is an operator
				if (ci->IsOperator())
				{
					// if the user to kill exists
					if (ul->UserExists(ci->GetContent()))
					{
						// close the socket and remove from user list
						closesocket(ul->GetUser(ci->GetContent())->GetSocket());
						ul->RemoveUser(ci->GetContent());
						send(ci->GetSocket(),
							server->SendTerminated(ci->GetContent()).c_str(),
							server->SendTerminated(ci->GetContent()).length(),
							0);
					}
					// user doesn't exist
					else
					{
						send(ci->GetSocket(),
							server->SendError(ci->GetContent()).c_str(),
							server->SendError(ci->GetContent()).length(),
							0);
					}
				}
				// else they're not an operator
				else
				{
					send(ci->GetSocket(),
						server->SendNotOperator().c_str(),
						server->SendNotOperator().length(),
						0);
				}
			}

			// if op, user is trying to gain operator status
			else if (strcmp(ci->GetCmd().c_str(), "/op") == 0)
			{
				// check password they enter .. i know not ideal, password would be stored
				// separately, but for proof of concept
				if (strcmp(ci->GetContent().c_str(), "totallysecretpassword") == 0)
				{
					send(ci->GetSocket(),
						server->SendOperator().c_str(),
						server->SendOperator().length(),
						0);
					ci->SetOperator();
				}
			}

			// if quit, send goobye, break and kill connection outside of loop
			else if (strcmp(ci->GetCmd().c_str(), "/quit") == 0)
			{
				send(ci->GetSocket(),
					server->SendGoodbye().c_str(),
					server->SendGoodbye().length(),
					0);
				break;
			}
		}

		// if no command, we're sending a message, so broadcast it only to the channel we're in
		else
		{
			cl->GetChannel(ci->GetCurrentChannel())->BroadcastMessage(ci->GetMsg());
		}
	}

	// remove from channel list
	cl->GetChannel(ci->GetCurrentChannel())->RemoveUser(ci->GetUsername());

	// remove from main list
	ul->RemoveUser(ci->GetUsername());

	// close socket
	closesocket(ci->GetSocket());

	// delete instance
	delete ci;
}

int main(int argc, char** argv)
{
	// start on port 50040
	INet4Address* clientAddress = new INet4Address(50040);

	ServerConnection* conn = new ServerConnection(clientAddress);

	// new userlist
	UserList* ul = new UserList();

	// new channel list
	ChannelList* cl = new ChannelList();

	// create main channel
	Channel* main = new Channel("main");

	// create the server
	Command* server = new Command();

	// empty the user list
	ul->Empty();

	// empty the channel list
	cl->Empty();

	// add main channel to the channel list
	cl->AddChannel(main);

	conn->StartServer(100);

	while (true)
	{
		ConnectionInstance* ci = conn->AcceptClient();
		std::cout << "Client connected" << std::endl;

		thread(ClientHandler, ci, ul, cl, server).detach();
	}

	delete main, clientAddress, conn, ul, cl, server;
}