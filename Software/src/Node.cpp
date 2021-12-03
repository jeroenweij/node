/*************************************************************
* Created by J. Weij
*************************************************************/

#include <avr/eeprom.h>
#include <pins.h>
#include <stdint.h>

#include "Arduino.h"
#include "ChannelManager.h"
#include "NodeLib/NodeComm.h"

using NodeLib::Node;

const int RXLED = 17;

Node node(PIN_ENABLE_485);
ChannelManager manager;

void setup(void)
{
    Serial.begin(115200);

    uint8_t nodeId = eeprom_read_byte((uint8_t*)0);
    Serial.println(nodeId);
    node.SetId(nodeId, RXLED);

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);
    pinMode(RXLED, OUTPUT);
    digitalWrite(RXLED, HIGH);

    manager.Init(node);

    node.RegisterHandler(&manager);
    node.Init();
}

void loop(void)
{
    manager.Loop();
    node.Loop();
}
