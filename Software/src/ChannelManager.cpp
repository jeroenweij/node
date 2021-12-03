/*************************************************************
* Created by J. Weij
*************************************************************/

#include <pins.h>

#include "Channel.h"
#include "ChannelManager.h"
#include "ServoChannel.h"

using NodeLib::Message;
using NodeLib::Node;

const int channelCount = 13;

Channel channels[channelCount] = {
    Channel(1, PIN_USER_1, ChannelType::DIGITAL, false),
    Channel(2, PIN_USER_2, ChannelType::DIGITAL, false),
    Channel(3, PIN_USER_3, ChannelType::DIGITAL, true),
    Channel(4, PIN_USER_4, ChannelType::DIGITAL, false),
    Channel(5, PIN_USER_5, ChannelType::DIGITAL, true),
    Channel(6, PIN_USER_6, ChannelType::DIGITAL, true),
    Channel(7, PIN_USER_7, ChannelType::ANALOG, false),
    Channel(8, PIN_USER_8, ChannelType::ANALOG, false),
    Channel(9, PIN_USER_9, ChannelType::ANALOG, false),
    ServoChannel(10, PIN_SERVO_1, ChannelType::SERVO, true),
    ServoChannel(11, PIN_SERVO_2, ChannelType::SERVO, true),
    Channel(12, PIN_MOSFET_1, ChannelType::MOSFET, true),
    Channel(13, PIN_MOSFET_2, ChannelType::MOSFET, true),
};

ChannelManager::ChannelManager()
{
}

void ChannelManager::Init(Node& node)
{
    for (Channel& c : channels)
    {
        c.Init(node);
    }
}

void ChannelManager::Loop()
{
    for (Channel& c : channels)
    {
        c.Loop();
    }
}

void ChannelManager::ReceivedMessage(const Message& m)
{
    if (m.id.channel > 0 && m.id.channel <= channelCount)
    {
        channels[m.id.channel - 1].HandleCommand(m);
    }
}
