#include "Arduino.h"

#include "NodeComm.h"

using NodeLib::Node;
using NodeLib::Message;

Node::Node(int nodeId, int enablePin) :
    frameStart({0xFF, 42}),
    nodeId(nodeId),
    enablePin(enablePin),
    sendRequest(nodeId, 0, Operation::SENDQ),
    handler(nullptr)
{ }

void Node::WriteMessage(NodeLib::Id, uint8_t value)
{ }

void Node::Init()
{
    pinMode(enablePin, OUTPUT);
    setEnable(false);
}

void Node::Loop()
{
    while (ReadMessage())
    { }
}

bool Node::ReadMessage()
{
    if (!Serial1.available() ||
      !Serial1.find((char*) &frameStart[0], sizeof(frameStart)))
    {
        return false;
    }
    Message m;
    size_t bytes = Serial1.readBytes((uint8_t*) &m, sizeof(m));

    if (bytes != sizeof(m))
    {
        return false;
    }

    if (m.id.node == nodeId)
    {
        if (m.id == sendRequest)
        {
            setEnable(true);
            flushQueue();
            setEnable(false);
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

void Node::flushQueue()
{ }

void Node::setEnable(bool enable)
{
    digitalWrite(enablePin, enable);
}

void Node::RegisterHandler(IVariableHandler* handler)
{
    this->handler = handler;
}
