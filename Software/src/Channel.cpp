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
    mode(PinMode::DIGITAL_IN),
    currentValue(0),
    node(nullptr),
    forceUpdate(false),
    nextUpdateTime(0)
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
            SetValue(m);
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
    node->QueueMessage({ node->GetId(), channel, operation, value });
}

void Channel::Init(Node& node)
{
    this->node = &node;

    switch (mode)
    {
        case PinMode::DIGITAL_IN:
        {
            pinMode(pin, INPUT_PULLUP);
            break;
        }
        case PinMode::DIGITAL_OUT:
        {
            pinMode(pin, OUTPUT);
            digitalWrite(pin, currentValue);
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

void Channel::Loop()
{
    if (millis() > nextUpdateTime || forceUpdate)
    {
        switch (mode)
        {
            case PinMode::ANALOG_IN:
            {
                Value value = analogRead(pin);
                if (value != currentValue || forceUpdate)
                {
                    currentValue = value;
                    QueueMessage(Operation::VALUE, value);
                    nextUpdateTime = millis() + debounceTime;
                }
                break;
            }
            case PinMode::DIGITAL_IN:
            {
                Value value = (digitalRead(pin) == LOW) ? 1 : 0;
                if (value != currentValue || forceUpdate)
                {
                    currentValue = value;
                    QueueMessage(Operation::VALUE, value);
                    nextUpdateTime = millis() + debounceTime;
                }
                break;
            }
            default:
                // Do nothing
                break;
        }
    }
    forceUpdate = false;
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

void Channel::SetValue(const Message& m)
{
    if (node == nullptr)
    {
        return;
    }

    Value value = 0;
    switch (mode)
    {
        case PinMode::SERVO:
        {
            currentValue = m.value;
            value = m.value;
            ServoWrite(m.value);
            break;
        }
        case PinMode::DIGITAL_OUT:
        {
            currentValue = m.value > 0 ? 1 : 0;
            value = currentValue;
            digitalWrite(pin, currentValue);
            break;
        }
        default:
            QueueMessage(Operation::ERROR);
            return;
            break;
    }
    QueueMessage(Operation::VALUE, value);
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
        value = currentValue;
        analogWrite(pin, currentValue);
    }
    else
    {
        QueueMessage(Operation::ERROR);
        return;
    }
    QueueMessage(Operation::VALUE, value);
}
