/*************************************************************
* Created by J. Weij
*************************************************************/

#pragma once

#include <Servo/Servo.h>

#include "Channel.h"

class ServoChannel : public Channel
{
  public:
    ServoChannel(const uint8_t channel, const int pin, const ChannelType type, const bool pwm);

  private:
    void ServoAttach() override;
    void ServoDetach() override;
    void ServoWrite(int value) override;

    Servo servo;
};
