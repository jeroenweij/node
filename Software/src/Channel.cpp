/*************************************************************
* Created by J. Weij
*************************************************************/

#include "Channel.h"
#include "Arduino.h"

Channel::Channel(const int pin, const ChannelType type, const bool pwm) :
    pin(pin),
    type(type),
    pwm(pwm),
    mode(PinMode::DIGITAL_IN),
    output(0)
{ }

void Channel::Init() const
{
    switch (mode) {
        case PinMode::DIGITAL_IN:
        {
            pinMode(pin, INPUT_PULLUP);
            break;
        }
        case PinMode::DIGITAL_OUT:
        {
            pinMode(pin, OUTPUT);
            digitalWrite(pin, output);
            break;
        }
        case PinMode::ANALOG_IN:
        {
            pinMode(pin, INPUT);
            break;
        }
        case PinMode::SERVO:
        {
            break;
        }
    }
}
