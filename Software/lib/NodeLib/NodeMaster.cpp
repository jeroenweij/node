/*************************************************************
* Created by J. Weij
*************************************************************/

#include "Arduino.h"
#include <tools/Logger.h>

#include "NodeMaster.h"
#include "id.h"

using NodeLib::ChannelId;
using NodeLib::IVariableHandler;
using NodeLib::Message;
using NodeLib::NodeMaster;
using NodeLib::Operation;

NodeMaster::NodeMaster(const int enablePin) :
    Node(enablePin),
    nodesFound(false)
{
    nodeId = masterNodeId;
}

void NodeMaster::Init()
{
    Node::Init();

    delay(500);
    DetectNodes();

    //Start polling nodes
    PollNextNode(0);
}

void NodeMaster::Loop()
{
    Node::Loop();
}

void NodeLib::NodeMaster::FlushNow()
{
    flushQueue();
}

void NodeMaster::DetectNodes()
{
    LOG_DEBUG("DetectNodes");
    setEnable(true);
    Message poll(0, Operation::DETECTNODES);
    WriteMessage(poll);
    setEnable(false);

    unsigned long endTime = millis() + (nodeSpacing * (numNodes + 1));
    while (millis() < endTime)
    {
        Node::Loop();
    }

    LOG_DEBUG("Done detecting");
}

void NodeMaster::PollNextNode(const int prevNodeId)
{
    // Flush any queued messages
    flushQueue();

    if (nodesFound)
    {
        // Determine next active node
        int nodeId = prevNodeId;
        do
        {
            nodeId++;
            if (nodeId > numNodes)
            {
                nodeId = 1;
            }

        } while (!activeNodes[nodeId - 1]);

        setEnable(true);
        Message poll(nodeId, Operation::SENDQ);
        WriteMessage(poll);
        setEnable(false);
    }
}

void NodeMaster::HandleInternalOperation(const Message& m)
{
    switch (m.id.operation)
    {
        case Operation::HELLOWORLD:
        {
            NodeHello(m.id.node);
            break;
        }
        case Operation::ENDOFQ:
        {
            PollNextNode(m.id.node);
            break;
        }
        default:
            break;
    }
}

void NodeMaster::HandleMasterMessage(const Message& m)
{
    if (m.id.channel == ChannelId::INTERNAL_MSG)
    {
        HandleInternalOperation(m);
    }
    else
    {
        if (handler)
        {
            handler->ReceivedMessage(m);
        }
    }
}

void NodeMaster::NodeHello(int nodeId)
{
    if (nodeId > 0 && nodeId <= numNodes)
    {
        LOG_INFO("Hello Node " << nodeId);
        activeNodes[nodeId - 1] = true;
        nodesFound = true;
    }
}
