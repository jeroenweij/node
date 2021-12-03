/*************************************************************
* Created by J. Weij
*************************************************************/

#pragma once

#include "IVariableHandler.h"
#include "id.h"

namespace NodeLib
{
    class Node
    {
      public:
        Node(int nodeId, int enablePin);

        void RegisterHandler(IVariableHandler* handler);
        void QueueMessage(const Id& id, const uint8_t value);

        void Init();
        void Loop();

      protected:
        bool ReadMessage();
        void flushQueue();
        void setEnable(bool enable);

      private:
        void WriteMessage(Message& m);
        void HandleInternalMessage(Message& m);

        const int nodeId;
        const int enablePin;
        const NodeLib::Id sendRequest;
        IVariableHandler* handler;
    };
}
