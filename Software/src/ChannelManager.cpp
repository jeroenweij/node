/*************************************************************
* Created by J. Weij
*************************************************************/

#include <pins.h>

#include "Channel.h"
#include "ChannelManager.h"
#include "ServoChannel.h"

using NodeLib::ChannelId;
using NodeLib::Message;
using NodeLib::Node;

const int channelCount = 13;

Channel channels[channelCount] = {
    Channel(ChannelId::DIGITAL_1, PIN_USER_1, ChannelType::DIGITAL, false),
    Channel(ChannelId::DIGITAL_2, PIN_USER_2, ChannelType::DIGITAL, false),
    Channel(ChannelId::DIGITAL_3, PIN_USER_3, ChannelType::DIGITAL, true),
    Channel(ChannelId::DIGITAL_4, PIN_USER_4, ChannelType::DIGITAL, false),
    Channel(ChannelId::DIGITAL_5, PIN_USER_5, ChannelType::DIGITAL, true),
    Channel(ChannelId::DIGITAL_6, PIN_USER_6, ChannelType::DIGITAL, true),
    Channel(ChannelId::ANALOG_1, PIN_USER_7, ChannelType::ANALOG, false),
    Channel(ChannelId::ANALOG_2, PIN_USER_8, ChannelType::ANALOG, false),
    Channel(ChannelId::ANALOG_3, PIN_USER_9, ChannelType::ANALOG, false),
    ServoChannel(ChannelId::SERVO_1, PIN_SERVO_1, ChannelType::SERVO, true),
    ServoChannel(ChannelId::SERVO_2, PIN_SERVO_2, ChannelType::SERVO, false),
    Channel(ChannelId::MOSFET_1, PIN_MOSFET_1, ChannelType::MOSFET, true),
    Channel(ChannelId::MOSFET_2, PIN_MOSFET_2, ChannelType::MOSFET, true),
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
    if (m.id.channel != ChannelId::INTERNAL_MSG)
    {
        channels[static_cast<int>(m.id.channel) - 1].HandleCommand(m);
    }
}
