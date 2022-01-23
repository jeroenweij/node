/*************************************************************
* Created by J. Weij
*************************************************************/

#include <avr/eeprom.h>
#include <pins.h>
#include <stdint.h>

#include <tools/Logger.h>

#include "Arduino.h"
#include "ChannelManager.h"
#include "NodeLib/Node.h"

using NodeLib::Node;

Node           node(PIN_ENABLE_485, PIN_LED, PIN_LED);
ChannelManager manager;

uint8_t EEMEM address = NODE_ID;

void setup(void)
{
    Serial.begin(115200);
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, HIGH);

    uint8_t nodeId = eeprom_read_byte((uint8_t*)&address);
    LOG_INFO("Node Id = " << nodeId);
    node.SetId(nodeId);

    manager.Init(node);

    node.RegisterHandler(&manager);
    node.Init();
    digitalWrite(PIN_LED, LOW);
    LOG_INFO(F("Node Initialised"));
}

void loop(void)
{
    manager.Loop();
    node.Loop();
}
