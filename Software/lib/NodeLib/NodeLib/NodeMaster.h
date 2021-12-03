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
        NodeMaster(int enablePin);

        void Init();
        void Loop();

      private:
    };
} // namespace NodeLib
