/*************************************************************
* Created by J. Weij
*************************************************************/

#include <stdint.h>
#include <pins.h>

#include "Arduino.h"
#include "NodeLib/NodeComm.h"
#include "ChannelManager.h"

using NodeLib::Node;

const int RXLED = 17;

Node node(1, PIN_ENABLE_485);
ChannelManager manager;

void setup(void)
{
    pinMode(RXLED, OUTPUT);
    digitalWrite(RXLED, HIGH);

    manager.Init();

    node.RegisterHandler(&manager);
    node.Init();
}

void loop(void)
{
    node.Loop();

    delay(500);
    digitalWrite(RXLED, LOW);
    delay(500);
    digitalWrite(RXLED, HIGH);

    delay(200);
}
