/*************************************************************
* Created by J. Weij
*************************************************************/

#include <Arduino.h>
#include <pins.h>

#include "Channel.h"
#include "ChannelManager.h"
#include "ServoChannel.h"

using NodeLib::ChannelId;
using NodeLib::Message;
using NodeLib::Node;

const int channelCount = 13;

Channel      c1(ChannelId::DIGITAL_1, PIN_USER_1, ChannelType::DIGITAL, false);
Channel      c2(ChannelId::DIGITAL_2, PIN_USER_2, ChannelType::DIGITAL, false);
Channel      c3(ChannelId::DIGITAL_3, PIN_USER_3, ChannelType::DIGITAL, true);
Channel      c4(ChannelId::DIGITAL_4, PIN_USER_4, ChannelType::DIGITAL, false);
ServoChannel c5(ChannelId::DIGITAL_5, PIN_USER_5, true);
Channel      c6(ChannelId::DIGITAL_6, PIN_USER_6, ChannelType::DIGITAL, true);
Channel      c7(ChannelId::ANALOG_1, PIN_USER_7, ChannelType::ANALOG, false);
Channel      c8(ChannelId::ANALOG_2, PIN_USER_8, ChannelType::ANALOG, false);
Channel      c9(ChannelId::ANALOG_3, PIN_USER_9, ChannelType::ANALOG, false);
ServoChannel c10(ChannelId::SERVO_1, PIN_SERVO_1, true);
ServoChannel c11(ChannelId::SERVO_2, PIN_SERVO_2, false);
Channel      c12(ChannelId::MOSFET_1, PIN_MOSFET_1, ChannelType::MOSFET, true);
Channel      c13(ChannelId::MOSFET_2, PIN_MOSFET_2, ChannelType::MOSFET, true);

Channel* channels[channelCount] = {&c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8, &c9, &c10, &c11, &c12, &c13};

ChannelManager::ChannelManager()
{
}

void ChannelManager::Init(Node& node)
{
    for (Channel* c : channels)
    {
        c->Init(node);
    }
}

void ChannelManager::Loop()
{
    for (Channel* c : channels)
    {
        c->Loop();
    }
}

void ChannelManager::ReceivedMessage(const Message& m)
{
    if (m.id.channel != ChannelId::INTERNAL_MSG)
    {
        channels[static_cast<int>(m.id.channel) - 1]->HandleCommand(m);
    }
}

void ChannelManager::ConnectionLost()
{
    digitalWrite(PIN_LED, HIGH);
    for (Channel* c : channels)
    {
        c->DisableOutput();
    }
}
