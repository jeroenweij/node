/*************************************************************
* Created by J. Weij
*************************************************************/

#include "Arduino.h"
#include <tools/DelayTimer.h>
#include <tools/Logger.h>

#include "NodeMaster.h"
#include "id.h"

using NodeLib::ChannelId;
using NodeLib::IVariableHandler;
using NodeLib::Message;
using NodeLib::NodeMaster;
using NodeLib::Operation;

NodeMaster::NodeMaster(const int enablePin, const int ledPin) :
    Node(enablePin, ledPin),
    nodesFound(false)
{
    nodeId = masterNodeId;
}

void NodeMaster::Init()
{
    Node::Init();

    delay(500);
    DetectNodes();
}

void NodeMaster::Loop()
{
    Node::Loop();
}

void NodeLib::NodeMaster::FlushNow(const bool force)
{
    if (force || messagesQueued > (queueSize * 8 / 10))
    {
        setEnable(true);
        flushQueue();
        setEnable(false);
    }
}

void NodeMaster::StartPollingNodes()
{
    //Start polling nodes
    PollNextNode(0);
}

void NodeMaster::DetectNodes()
{
    LOG_INFO(F("Detecting Nodes"));
    setEnable(true);
    Message poll(0, Operation::DETECTNODES);
    WriteMessage(poll);
    setEnable(false);

    DelayTimer timeout(nodeSpacing * (numNodes + 1));
    while (timeout.isRunning() && !timeout.Finished())
    {
        Node::Loop();
    }

    LOG_INFO(F("Done Detecting Nodes"));
}

void NodeMaster::PollNextNode(const int prevNodeId)
{
    // Flush any queued messages
    setEnable(true);
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

        Message poll(nodeId, Operation::SENDQ);
        WriteMessage(poll);
    }
    setEnable(false);
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
        nodesFound              = true;
    }
}
