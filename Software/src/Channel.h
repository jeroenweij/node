/*************************************************************
* Created by J. Weij
*************************************************************/

#pragma once

#include "NodeLib/EPinMode.h"
#include "NodeLib/Node.h"
#include "NodeLib/id.h"

enum class ChannelType
{
    DIGITAL,
    ANALOG,
    SERVO,
    MOSFET
};

class Channel
{
  public:
    Channel(const NodeLib::ChannelId channel, const int pin, const ChannelType type, const bool pwm);

    void HandleCommand(const NodeLib::Message& m);
    void Init(NodeLib::Node& node);
    void Loop();
    void DisableOutput();

  protected:
    virtual void ServoAttach() {}
    virtual void ServoDetach() {}
    virtual void ServoWrite(int);

    const int pin;

  private:
    void GetValue(const NodeLib::Message& m);
    void SetValue(Value newValue);
    void SetValuePwm(const NodeLib::Message& m);
    void SetMode(const NodeLib::Message& m, const NodeLib::PinMode newMode);
    void QueueMessage(const NodeLib::Operation operation);
    void QueueMessage(const NodeLib::Operation operation, const Value value);

    const NodeLib::ChannelId channel;
    const ChannelType        type;
    const bool               pwm;
    NodeLib::PinMode         mode;
    Value                    currentValue;
    NodeLib::Node*           node;
    bool                     forceUpdate;
    unsigned long            nextUpdateTime;
};
