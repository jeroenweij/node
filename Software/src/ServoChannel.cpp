/*************************************************************
* Created by J. Weij
*************************************************************/


#include "ServoChannel.h"

ServoChannel::ServoChannel(const uint8_t channel, const int pin, const ChannelType type, const bool pwm)
    : Channel(channel, pin, type, pwm)
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
