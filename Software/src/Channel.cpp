/*************************************************************
* Created by J. Weij
*************************************************************/

#include "Arduino.h"
#include <tools/Logger.h>

#include "Channel.h"

using NodeLib::ChannelId;
using NodeLib::Message;
using NodeLib::Node;
using NodeLib::Operation;
using NodeLib::PinMode;

static const int debounceTime = 100;

Channel::Channel(const ChannelId channel, const int pin, const ChannelType type, const bool pwm) :
    pin(pin),
    channel(channel),
    type(type),
    pwm(pwm),
    mode(PinMode::NOT_CONFIGURED),
    currentValue(0),
    node(nullptr),
    forceUpdate(false),
    nextUpdateTime()
{
}

void Channel::HandleCommand(const NodeLib::Message& m)
{
    switch (m.id.operation)
    {
        case Operation::GET:
        {
            GetValue(m);
            break;
        }
        case Operation::SET:
        {
            SetValue(m.value);
            break;
        }
        case Operation::SETPWM:
        {
            SetValuePwm(m);
            break;
        }
        case Operation::SETMODE:
        {
            SetMode(m, static_cast<PinMode>(m.value));
            break;
        }
        default:
            break;
    }
}

void Channel::SetMode(const Message& m, const PinMode newMode)
{
    LOG_INFO("Set mode, channel: " << m.id.channel << ", Mode: " << mode << "->" << newMode);
    forceUpdate = true;

    if (mode == newMode)
    {
        return;
    }

    if (mode == PinMode::SERVO)
    {
        ServoDetach();
    }

    currentValue = 0;
    switch (newMode)
    {
        case PinMode::ANALOG_IN:
            if (type != ChannelType::ANALOG)
            {
                QueueMessage(Operation::ERROR);
            }
            else
            {
                mode = newMode;
                pinMode(pin, INPUT);
                forceUpdate = true;
            }
            break;
        case PinMode::DIGITAL_IN:
            if (type == ChannelType::MOSFET)
            {
                QueueMessage(Operation::ERROR);
            }
            else
            {
                mode = newMode;
                pinMode(pin, INPUT_PULLUP);
                forceUpdate = true;
            }
            break;
        case PinMode::DIGITAL_OUT:
        {
            mode = newMode;
            pinMode(pin, OUTPUT);
            digitalWrite(pin, currentValue);
            break;
        }
        case PinMode::SERVO:
            if (type != ChannelType::SERVO)
            {
                QueueMessage(Operation::ERROR);
            }
            else
            {
                mode = newMode;
                pinMode(pin, OUTPUT);
                ServoAttach();
                ServoWrite(currentValue);
            }
            break;
    }
}

void Channel::QueueMessage(const Operation operation)
{
    QueueMessage(operation, 0);
}

void Channel::QueueMessage(const NodeLib::Operation operation, const Value value)
{
    if (node == nullptr)
    {
        return;
    }
    node->QueueMessage({node->GetId(), channel, operation, value});
}

void Channel::Init(Node& node)
{
    this->node = &node;

    pinMode(pin, INPUT);
    mode = PinMode::NOT_CONFIGURED;
}

void Channel::Loop()
{
    switch (mode)
    {
        case PinMode::ANALOG_IN:
        {
            if (!nextUpdateTime.isRunning() || nextUpdateTime.Finished())
            {
                Value value = analogRead(pin);
                if (value != currentValue || forceUpdate)
                {
                    currentValue = value;
                    QueueMessage(Operation::VALUE, value);
                    nextUpdateTime.Start(debounceTime);
                }
            }
            break;
        }
        case PinMode::DIGITAL_IN:
        {
            if (!nextUpdateTime.isRunning() || nextUpdateTime.Finished())
            {
                Value value = (digitalRead(pin) == LOW) ? 1 : 0;
                if (value != currentValue || forceUpdate)
                {
                    currentValue = value;
                    QueueMessage(Operation::VALUE, value);
                    nextUpdateTime.Start(debounceTime);
                }
            }
            break;
        }
        default:
            // Do nothing
            break;
    }
    forceUpdate = false;
}

void Channel::DisableOutput()
{
    if (mode == PinMode::DIGITAL_OUT)
    {
        SetValue(0);
    }
}

void Channel::ServoWrite(int)
{
    LOG_ERROR("Not a servo channel");
}

void Channel::GetValue(const Message& m)
{
    if (node == nullptr)
    {
        return;
    }
    Value value = 0;

    switch (mode)
    {
        case PinMode::ANALOG_IN:
        {
            value = analogRead(pin);
            break;
        }
        case PinMode::DIGITAL_IN:
        {
            value = (digitalRead(pin) == LOW) ? 1 : 0;
            break;
        }
        default:
            value = currentValue;
            break;
    }
    QueueMessage(Operation::VALUE, value);
}

void Channel::SetValue(Value newValue)
{
    if (node == nullptr)
    {
        return;
    }

    switch (mode)
    {
        case PinMode::SERVO:
        {
            currentValue = newValue;
            ServoWrite(currentValue);
            break;
        }
        case PinMode::DIGITAL_OUT:
        {
            currentValue = newValue > 0 ? 1 : 0;
            newValue     = currentValue;
            digitalWrite(pin, currentValue);
            break;
        }
        default:
            QueueMessage(Operation::ERROR);
            return;
            break;
    }
    QueueMessage(Operation::VALUE, newValue);
}

void Channel::SetValuePwm(const Message& m)
{
    if (node == nullptr)
    {
        return;
    }
    Value value = 0;

    if (pwm)
    {
        if (mode != PinMode::DIGITAL_OUT)
        {
            SetMode(m, PinMode::DIGITAL_OUT);
        }
        currentValue = m.value;
        value        = currentValue;
        analogWrite(pin, currentValue);
    }
    else
    {
        QueueMessage(Operation::ERROR);
        return;
    }
    QueueMessage(Operation::VALUE, value);
}
