/*************************************************************
* Created by J. Weij
*************************************************************/

#pragma once

#include "Node.h"

namespace NodeLib
{

    class NodeMaster : public Node
    {
      public:
        NodeMaster(const int enablePin, const int ledPin, const int errorLedPin, const int buttonPin);

        void Init(const uint8_t expectedNumNodes);
        void Loop();
        void FlushNow(const bool force = false);
        void StartPollingNodes();

      private:
        void          DetectNodes();
        const uint8_t ActiveNodeCount() const;
        void          PollNextNode(const int prevNodeId);
        void          HandleMasterMessage(const Message& m) override;
        void          NodeHello(int nodeId);
        void          HandleInternalOperation(const Message& m);

        bool activeNodes[numNodes];
        bool nodesFound;
    };
} // namespace NodeLib
