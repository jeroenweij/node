/*************************************************************
* Created by J. Weij
*************************************************************/

#include "ServoChannel.h"

using NodeLib::ChannelId;

ServoChannel::ServoChannel(const ChannelId channel, const int pin, const bool pwm) :
    Channel(channel, pin, ChannelType::SERVO, pwm)
{
}

void ServoChannel::ServoAttach()
{
    servo.attach(pin);
}

void ServoChannel::ServoDetach()
{
    servo.detach();
}

void ServoChannel::ServoWrite(int value)
{
    servo.write(value);
}
