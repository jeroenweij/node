/*************************************************************
* Created by J. Weij
*************************************************************/

#pragma once

#include "id.h"

#include <stdint.h>

namespace NodeLib
{

    class IVariableHandler
    {
      public:
        virtual void ReceivedMessage(const Message& message) = 0;
        virtual void ConnectionLost()                        = 0;
    };
} // namespace NodeLib
