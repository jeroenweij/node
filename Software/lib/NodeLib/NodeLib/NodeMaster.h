#pragma once

#include "NodeComm.h"

namespace NodeLib
{
    class NodeMaster : Node
    {
public:
        using Node::WriteMessage;

        NodeMaster(int enablePin);

        void Init();
        void Loop();

private:
    };
}
