/*************************************************************
* Created by J. Weij
*************************************************************/

#pragma once

#include <NodeLib/IVariableHandler.h>
#include <NodeLib/id.h>
#include <NodeLib/NodeComm.h>

class ChannelManager : public NodeLib::IVariableHandler
{
public:
    ChannelManager();
    void Init(NodeLib::Node &node);
    void Loop();

    void ReceivedMessage(const NodeLib::Message& m);
private:
};
