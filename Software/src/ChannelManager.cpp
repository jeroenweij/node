/*************************************************************
* Created by J. Weij
*************************************************************/

#include <pins.h>

#include "Channel.h"
#include "ChannelManager.h"

using NodeLib::Message;

const int channelCount = 13;

Channel channels[channelCount] = {
    { PIN_USER_1, ChannelType::DIGITAL, false },
    { PIN_USER_2, ChannelType::DIGITAL, false },
    { PIN_USER_3, ChannelType::DIGITAL, true },
    { PIN_USER_4, ChannelType::DIGITAL, false },
    { PIN_USER_5, ChannelType::DIGITAL, true },
    { PIN_USER_6, ChannelType::DIGITAL, true },
    { PIN_USER_7, ChannelType::ANALOG, false },
    { PIN_USER_8, ChannelType::ANALOG, false },
    { PIN_USER_9, ChannelType::ANALOG, false },
    { PIN_SERVO_1, ChannelType::SERVO, true },
    { PIN_SERVO_2, ChannelType::SERVO, true },
    { PIN_MOSFET_1, ChannelType::POWER, true },
    { PIN_MOSFET_2, ChannelType::POWER, true },
};

ChannelManager::ChannelManager()
{
}

void ChannelManager::Init()
{
    for (const Channel& c : channels)
    {
        c.Init();
    }
}

void ChannelManager::ReceivedMessage(const Message& message)
{
}
