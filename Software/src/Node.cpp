#include <stdint.h>

#include "Arduino.h"
#include "NodeLib/NodeComm.h"

using NodeLib::Node;

const int RXLED = 17;

Node node(1, 2);

void setup(void)
{
    pinMode(RXLED, OUTPUT);
    digitalWrite(RXLED, HIGH);

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
