#include "NodeMaster.h"

using NodeLib::NodeMaster;
using NodeLib::IVariableHandler;

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
