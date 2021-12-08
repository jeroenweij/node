/*************************************************************
* Created by J. Weij
*************************************************************/

#include <avr/eeprom.h>
#include <pins.h>
#include <stdint.h>


#include <tools/Logger.h>

#include "Arduino.h"
#include "ChannelManager.h"
#include "NodeLib/NodeComm.h"

using NodeLib::Node;

Node node(PIN_ENABLE_485);
ChannelManager manager;

void setup(void)
{
    Serial.begin(115200);
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, HIGH);

    uint8_t nodeId = eeprom_read_byte((uint8_t*)0);
    Serial.println(nodeId);
    node.SetId(nodeId, PIN_LED);

    digitalWrite(PIN_LED, LOW);

    manager.Init(node);

    node.RegisterHandler(&manager);
    node.Init();
    LOG_INFO(F("Node Initialised"));
}

void loop(void)
{
    manager.Loop();
    node.Loop();
}
