/*************************************************************
* Created by J. Weij
*************************************************************/

#include "Arduino.h"

#include "NodeComm.h"

using NodeLib::Id;
using NodeLib::Message;
using NodeLib::Node;

const uint8_t frameStart[2] = { 0xFF, 42 };

Node::Node(int nodeId, int enablePin) :
    nodeId(nodeId),
    enablePin(enablePin),
    sendRequest(nodeId, 0, Operation::SENDQ),
    handler(nullptr)
{
}

void Node::QueueMessage(const Id& id, const uint8_t value)
{
    Serial1.write(frameStart, sizeof(frameStart));
}

void Node::WriteMessage(Message& m)
{
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

bool Node::ReadMessage()
{
    if (!Serial1.available() || !Serial1.find((char*)&frameStart[0], sizeof(frameStart)))
    {
        return false;
    }
    Message m;
    size_t bytes = Serial1.readBytes((uint8_t*)&m, sizeof(m));

    if (bytes != sizeof(m))
    {
        return false;
    }

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
}

void Node::HandleInternalMessage(Message& m)
{
    switch (m.id.operation)
    {
        case Operation::SENDQ:
        {
            setEnable(true);
            flushQueue();
            setEnable(false);
            break;
        }
        default:
            // TODO
            break;
    }
}

void Node::flushQueue()
{
}

void Node::setEnable(bool enable)
{
    digitalWrite(enablePin, enable);
}

void Node::RegisterHandler(IVariableHandler* handler)
{
    this->handler = handler;
}
