/*************************************************************
* Created by J. Weij
*************************************************************/

#pragma once

#include "NodeLib/EPinMode.h"
#include "NodeLib/NodeComm.h"
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
    Channel(const uint8_t channel, const int pin, const ChannelType type, const bool pwm);

    void HandleCommand(const NodeLib::Message& m);
    void Init(NodeLib::Node& node);
    void Loop();

  protected:
    virtual void ServoAttach() { }
    virtual void ServoDetach() { }
    virtual void ServoWrite(int) { }

    const int pin;

  private:
    void GetValue(const NodeLib::Message& m);
    void SetValue(const NodeLib::Message& m);
    void SetValuePwm(const NodeLib::Message& m);
    void SetMode(const NodeLib::Message& m, const NodeLib::PinMode newMode);
    void QueueMessage(const NodeLib::Operation operation);
    void QueueMessage(const NodeLib::Operation operation, const Value value);

    const uint8_t channel;
    const ChannelType type;
    const bool pwm;
    NodeLib::PinMode mode;
    Value currentValue;
    NodeLib::Node* node;
    bool forceUpdate;
};
