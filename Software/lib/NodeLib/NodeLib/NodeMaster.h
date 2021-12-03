/*************************************************************
* Created by J. Weij
*************************************************************/

#pragma once

#include "NodeComm.h"

namespace NodeLib
{

    class NodeMaster : public Node
    {
      public:
        NodeMaster(const int enablePin, const int ledPin);

        void Init();
        void Loop();
        void FlushNow();

      private:
        void DetectNodes();
        void PollNextNode(const int prevNodeId);
        void HandleMasterMessage(const Message& m) override;
        void NodeHello(int nodeId);

        bool activeNodes[numNodes];
    };
} // namespace NodeLib
