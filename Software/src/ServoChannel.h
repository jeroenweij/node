/*************************************************************
* Created by J. Weij
*************************************************************/

#pragma once

#include <Servo/Servo.h>

#include "Channel.h"

class ServoChannel : public Channel
{
  public:
    ServoChannel(const NodeLib::ChannelId channel, const int pin, const bool pwm);

  private:
    void ServoAttach() override;
    void ServoDetach() override;
    void ServoWrite(int value) override;

    Servo servo;
};
