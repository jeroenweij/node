/*************************************************************
* Created by J. Weij
*************************************************************/

#pragma once

enum class ChannelType
{
    DIGITAL,
    ANALOG,
    SERVO,
    POWER
};

class Channel
{
  public:
    Channel(const int pin, const ChannelType type, const bool pwm);

    void Init() const;
  private:
    enum class PinMode{
        DIGITAL_IN,
        DIGITAL_OUT,
        ANALOG_IN,
        SERVO,
    };

    const int pin;
    const ChannelType type;
    const bool pwm;
    PinMode mode;
    int output;
};
