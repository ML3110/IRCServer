#pragma once
#include <vector>
#include "Channel.h"

class ChannelList
{
	// Attributes
private:
	std::vector<Channel*> channels;

	// Constructors
public:
	ChannelList();
	~ChannelList();

	// Properties
	int GetCount();

	// Methods
	void Empty();
	void AddChannel(Channel* channel);
	std::string Print(std::string currentUserChannel);
	Channel* GetChannel(std::string channelName);
	bool ChannelExists(std::string channelName);
};