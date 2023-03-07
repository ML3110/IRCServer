#pragma once
#include "ChannelList.h"

ChannelList::ChannelList()
{

}

ChannelList::~ChannelList()
{

}

// empty the list
void ChannelList::Empty()
{
	this->channels.empty();
}

// add channel to the list
void ChannelList::AddChannel(Channel* channel)
{
	this->channels.push_back(channel);
}

// get the current channels
std::string ChannelList::Print(std::string currentUserChannel)
{
	std::string currentChannels;

	currentChannels += "=====================================\r\nCurrent channels available:\r\n=====================================\r\n";

	// for each channel, add to str. 
	std::for_each(channels.begin(), channels.end(), [&currentChannels, &currentUserChannel](Channel* channel) {
		currentChannels += channel->GetName();
		// if channel is where the user currently is, append that too to let them know
		if (channel->GetName() == currentUserChannel)
		{
			currentChannels += " <-- You are here";
		}
		currentChannels += "\r\n";
		});

	currentChannels += "=====================================\r\n";

	return currentChannels;
}

Channel* ChannelList::GetChannel(std::string channelName)
{
	// find the channel, set iterator
	auto it = std::find_if(channels.begin(), channels.end(), [&channelName](Channel* channels) {
		return channelName == channels->GetName();
		});

	// return the distance from the start of the vector to the iterator
	return channels[std::distance(channels.begin(), it)];
}

// Check if the channel exists
bool ChannelList::ChannelExists(std::string channelName)
{
	// find channel, set iterator
	auto it = std::find_if(channels.begin(), channels.end(), [&channelName](Channel* channels) {
		return channelName == channels->GetName();
		});

	// if iterator has not hit the end, channel exists
	if (it != channels.end())
	{
		return true;
	}

	// if iterator is at the end, channel doesn't exist
	else
	{
		return false;
	}
}

// return count of list size
int ChannelList::GetCount()
{
	return this->channels.size();
}