/*************************************************************
* Created by J. Weij
*************************************************************/

#pragma once

#include <NodeLib/IVariableHandler.h>
#include <NodeLib/id.h>

class ChannelManager : public NodeLib::IVariableHandler
{
public:
    ChannelManager();
    void Init();

    void ReceivedMessage(const NodeLib::Message& message);
private:
};
