/*************************************************************
* Created by J. Weij
*************************************************************/

#pragma once

#include <tools/DelayTimer.h>

#include "ErrorHandler.h"
#include "IVariableHandler.h"
#include "id.h"

namespace NodeLib
{
    static const int numNodes = 10;

    class Node
    {
      public:
        Node(const int enablePin, const int ledPin, const int errorLedPin, const int buttonPin = -1);

        void RegisterHandler(IVariableHandler* handler);
        void QueueMessage(const Message& m);
        void QueueMessage(const Id& id, const Value value);

        void    SetId(const uint8_t newId);
        uint8_t GetId();

        void Init();
        void Loop();

      protected:
        bool         ReadMessage(Message& m);
        void         flushQueue();
        void         setEnable(bool enable);
        void         WriteMessage(const Message& m);
        void         ResetHearthBeat();
        virtual void HandleMasterMessage(const Message& m) {}

        ErrorHandler         errorHandler;
        static const int     queueSize    = 25;
        static const uint8_t masterNodeId = 0;
        static const int     nodeSpacing  = 25;

        IVariableHandler* handler;
        uint8_t           nodeId;
        int               messagesQueued;

      private:
        void HandlePollRequest();
        void HandleMessage(const Message& m);
        void HandleInternalMessage(const Message& m);

        const int  enablePin;
        const int  ledPin;
        Message    messageQueue[queueSize];
        DelayTimer hearthBeatTimer;
    };
} // namespace NodeLib
