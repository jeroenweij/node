/*************************************************************
* Created by J. Weij
*************************************************************/

#include "Arduino.h"
#include <tools/Logger.h>

#include "Node.h"

using NodeLib::ChannelId;
using NodeLib::Id;
using NodeLib::Message;
using NodeLib::Node;
using NodeLib::numNodes;
using NodeLib::Operation;

byte frameStart[2] = {0xFF, 0x42};

Node::Node(const int enablePin, const int ledPin) :
    handler(nullptr),
    nodeId(99),
    enablePin(enablePin),
    ledPin(ledPin),
    messagesQueued(0)
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

    Serial1.begin(9600);
}

void Node::Loop()
{
    while (Serial1.available())
    {
        if (FindFrameStart())
        {
            ReadMessage();
        }
    }
}

bool Node::FindFrameStart()
{
    int           findByte = 0;
    unsigned long timeout  = millis() + 100;

    while (millis() < timeout)
    {
        uint8_t b = Serial1.read();
        if (b == frameStart[findByte])
        {
            findByte++;
            if (findByte == sizeof(frameStart))
            {
                return true;
            }
        }
        else
        {
            if (findByte > 0 && b == frameStart[0])
            {
                findByte = 1;
            }
            else
            {
                if (b == -1)
                {
                    return false;
                }
                findByte = 0;
            }
        }
    }
    return false;
}

bool Node::ReadMessage()
{
    Message m;
    size_t  bytes = Serial1.readBytes((uint8_t*)&m, sizeof(m));

    if (bytes != sizeof(m))
    {
        LOG_ERROR("Invalid length");
        return false;
    }

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
            flushQueue();
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
    setEnable(true);
    for (int i = 0; i < messagesQueued; i++)
    {
        WriteMessage(messageQueue[i]);

        // Limit rate so reader can keep up
        delay(10);
    }
    messagesQueued = 0;

    if (nodeId > 0)
    {
        Message end(nodeId, Operation::ENDOFQ);
        WriteMessage(end);
    }
    setEnable(false);
    digitalWrite(ledPin, LOW);
}

void Node::setEnable(bool enable)
{
    static const int delayMs = 10;
    if (!enable)
        delay(delayMs);
    digitalWrite(enablePin, enable);
    if (enable)
        delay(delayMs + 5);
}

void Node::HandlePollRequest()
{
    LOG_INFO(F("Handle poll request"));
    if (nodeId > 0)
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
        while (true)
        {
            LOG_ERROR("Invalid Node Id: " << nodeId);
            digitalWrite(ledPin, HIGH);
            delay(500);
            digitalWrite(ledPin, LOW);
            delay(500);
        }
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
