/*************************************************************
* Created by J. Weij
*************************************************************/

#include "Arduino.h"
#include <tools/Logger.h>

#include "NodeComm.h"

using NodeLib::Id;
using NodeLib::Message;
using NodeLib::Node;
using NodeLib::numNodes;
using NodeLib::Operation;

byte frameStart[2] = { 0xFF, 0x42 };

Node::Node(const int enablePin) :
    nodeId(99),
    enablePin(enablePin),
    sendRequest(nodeId, 0, Operation::SENDQ),
    handler(nullptr),
    messagesQueued(0)
{
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
    setEnable(false);

    Serial1.begin(9600);
}

void Node::Loop()
{
    while (ReadMessage())
    {
        // Work is done in ReadMessage()
    }
}

bool find()
{
    int findByte = 0;

    long timeout = millis() + 100;

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
    if (!Serial1.available() || !find())
    {
        return false;
    }

    Message m;
    size_t bytes = Serial1.readBytes((uint8_t*)&m, sizeof(m));

    if (bytes != sizeof(m))
    {
        LOG_ERROR("Invalid length");
        return false;
    }

    LOG_DEBUG("READ  Node: " << m.id.node << " Chan: " << m.id.channel << " Op: " << m.id.operation << " V: " << m.value);

    if (m.id.node == nodeId)
    {
        if (m.id.channel == 0)
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
    else if (m.id.node == masterNodeId && m.id.operation == Operation::POLLNODES)
    {
        HandlePollRequest();
    }
    // Internal message to master node
    else if (nodeId == masterNodeId && m.id.channel == 0)
    {
        HandleMasterMessage(m);
    }

    return true;
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
    LOG_DEBUG("Flush queue");

    setEnable(true);
    for (int i = 0; i < messagesQueued; i++)
    {
        WriteMessage(messageQueue[i]);
    }
    messagesQueued = 0;

    if (nodeId > 0)
    {
        Message end(nodeId, Operation::ENDOFQ);
        WriteMessage(end);
    }
    setEnable(false);
}

void Node::setEnable(bool enable)
{
    digitalWrite(enablePin, enable);
}

void Node::HandlePollRequest()
{
    LOG_INFO("Handle poll request");
    if (nodeId > 0)
    {
        digitalWrite(17, LOW);
        const Message m(nodeId, Operation::HELLOWORLD);
        delay((nodeId - 1) * nodeSpacing);
        setEnable(true);
        WriteMessage(m);
        setEnable(false);
    }
}

void Node::RegisterHandler(IVariableHandler* handler)
{
    this->handler = handler;
}

void NodeLib::Node::SetId(const uint8_t newId, const int ledPin)
{
    nodeId = newId;
    if (nodeId == 0 || nodeId > numNodes)
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
    QueueMessage({ id, value });
}

void Node::QueueMessage(const NodeLib::Message& m)
{
    if (messagesQueued < queueSize)
    {
        messageQueue[messagesQueued] = m;
        messagesQueued++;
    }
}