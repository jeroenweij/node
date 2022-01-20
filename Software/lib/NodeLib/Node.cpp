/*************************************************************
* Created by J. Weij
*************************************************************/

#include "Arduino.h"
#include <tools/DelayTimer.h>
#include <tools/Logger.h>

#include "Node.h"

using NodeLib::ChannelId;
using NodeLib::Id;
using NodeLib::Message;
using NodeLib::Node;
using NodeLib::numNodes;
using NodeLib::Operation;

uint8_t frameStart[2] = {0xEE, 0x42};

Node::Node(const int enablePin, const int ledPin, const int errorLedPin, const int buttonPin) :
    errorHandler(errorLedPin, buttonPin),
    handler(nullptr),
    nodeId(99),
    messagesQueued(0),
    enablePin(enablePin),
    ledPin(ledPin),
    hearthBeatTimer()
{
    pinMode(enablePin, OUTPUT);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    digitalWrite(enablePin, LOW);
}

void Node::WriteMessage(const Message& m)
{
    LOG_DEBUG("WRITE Node: " << m.id.node << " Chan: " << m.id.channel << " Op: " << m.id.operation << " V: " << m.value);
    Serial1.write(frameStart, sizeof(frameStart));
    Serial1.write((uint8_t*)&m, sizeof(m));
}

void Node::Init()
{
    pinMode(enablePin, OUTPUT);
    pinMode(ledPin, OUTPUT);
    digitalWrite(enablePin, LOW);

    Serial1.begin(115200);
}

void Node::Loop()
{
    struct Frame
    {
        uint8_t start[sizeof(frameStart)];
        Message message;
    };

    static uint8_t buffer[sizeof(Frame)] = {0};
    static int     atByte                = 0;
    static Frame*  frame                 = (Frame*)buffer;

    while (Serial1.available())
    {
        buffer[atByte] = Serial1.read();

        if (atByte < sizeof(frameStart))
        {
            if (frame->start[atByte] == frameStart[atByte])
            {
                atByte++;
            }
            else
            {
                atByte = 0;
            }
        }
        else
        {
            atByte++;
        }

        if (atByte >= sizeof(buffer))
        {
            ReadMessage(frame->message);
            atByte = 0;
        }
    }

    if (hearthBeatTimer.Finished() && handler != nullptr)
    {
        handler->ConnectionLost();
    }
}

void Node::ResetHearthBeat()
{
    hearthBeatTimer.Start(500);
}

bool Node::ReadMessage(Message& m)
{
    LOG_DEBUG("READ  Node: " << m.id.node << " Chan: " << m.id.channel << " Op: " << m.id.operation << " V: " << m.value);

    if (nodeId != masterNodeId)
    {
        HandleMessage(m);
    }
    else
    {
        HandleMasterMessage(m);
    }

    return true;
}

void Node::HandleMessage(const Message& m)
{
    if (m.id.node == nodeId)
    {
        if (m.id.channel == ChannelId::INTERNAL_MSG)
        {
            HandleInternalMessage(m);
        }
        else
        {
            if (handler)
            {
                handler->ReceivedMessage(m);
            }
        }
    }
    // Polrequest from master node
    else if (m.id.node == masterNodeId && m.id.operation == Operation::DETECTNODES)
    {
        HandlePollRequest();
    }
}

void Node::HandleInternalMessage(const Message& m)
{
    LOG_DEBUG("Handle internal operation: " << m.id.operation);
    switch (m.id.operation)
    {
        case Operation::SENDQ:
        {
            setEnable(true);
            flushQueue();
            setEnable(false);
            ResetHearthBeat();
            break;
        }
        default:
            // Do nothing
            break;
    }
}

void Node::flushQueue()
{
    LOG_DEBUG(F("Flush queue"));

    digitalWrite(ledPin, HIGH);
    for (int i = 0; i < messagesQueued; i++)
    {
        WriteMessage(messageQueue[i]);

        if ((i + 1 < messagesQueued) && (i % 5 == 0))
        {
            // Limit rate so reader can keep up
            delay(5);
        }
    }
    messagesQueued = 0;

    if (nodeId != masterNodeId)
    {
        Message end(nodeId, Operation::ENDOFQ);
        WriteMessage(end);
    }
    digitalWrite(ledPin, LOW);
}

void Node::setEnable(bool enable)
{
    static const int delayMs = 10;

    if (!enable)
    {
        delay(delayMs);
    }

    digitalWrite(enablePin, enable);

    if (enable)
    {
        delay(delayMs + 5);
    }
}

void Node::HandlePollRequest()
{
    LOG_INFO(F("Handle poll request"));
    if (nodeId != masterNodeId)
    {
        const Message m(nodeId, Operation::HELLOWORLD);
        delay((nodeId - 1) * nodeSpacing);
        LOG_INFO(F("Return Hello world"));
        setEnable(true);
        WriteMessage(m);
        setEnable(false);
    }
}

void Node::RegisterHandler(IVariableHandler* handler)
{
    this->handler = handler;
}

void NodeLib::Node::SetId(const uint8_t newId)
{
    nodeId = newId;
    if (nodeId == masterNodeId || nodeId > numNodes)
    {
        LOG_ERROR("Invalid Node Id: " << nodeId);
        errorHandler.Error(false);
    }
}

uint8_t Node::GetId()
{
    return nodeId;
}

void Node::QueueMessage(const Id& id, const Value value)
{
    QueueMessage({id, value});
}

void Node::QueueMessage(const NodeLib::Message& m)
{
    if (messagesQueued < queueSize)
    {
        messageQueue[messagesQueued] = m;
        messagesQueued++;
    }
}
