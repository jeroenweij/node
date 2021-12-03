/*************************************************************
* Created by J. Weij
*************************************************************/

#include "NodeMaster.h"

using NodeLib::IVariableHandler;
using NodeLib::NodeMaster;

NodeMaster::NodeMaster(int enablePin) :
    Node(0, enablePin)
{
}

void NodeMaster::Init()
{
    Node::Init();
}

void NodeMaster::Loop()
{
}
