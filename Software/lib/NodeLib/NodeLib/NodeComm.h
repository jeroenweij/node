/*************************************************************
* Created by J. Weij
*************************************************************/

#pragma once

#include "IVariableHandler.h"
#include "id.h"

namespace NodeLib
{
    static const int numNodes = 10;

    class Node
    {
      public:
        Node(const int enablePin);

        void RegisterHandler(IVariableHandler* handler);
        void QueueMessage(const Message& m);
        void QueueMessage(const Id& id, const Value value);

        void SetId(const uint8_t newId, const int ledPin);
        uint8_t GetId();

        void Init();
        void Loop();

      protected:
        static const uint8_t masterNodeId = 0;
        static const int nodeSpacing = 150;

        bool ReadMessage();
        void flushQueue();
        void setEnable(bool enable);
        void WriteMessage(const Message& m);
        virtual void HandleMasterMessage(const Message& m) { }

      private:
        void HandlePollRequest();
        void HandleInternalMessage(const Message& m);

        static const int queueSize = 25;
        uint8_t nodeId;
        const int enablePin;
        const NodeLib::Id sendRequest;
        IVariableHandler* handler;
        Message messageQueue[queueSize];
        int messagesQueued;
    };
}
