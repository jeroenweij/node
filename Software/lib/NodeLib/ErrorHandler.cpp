/*************************************************************
* Created by J. Weij
*************************************************************/

#include "Arduino.h"
#include <tools/Logger.h>

#include "ErrorHandler.h"

using NodeLib::ErrorHandler;

ErrorHandler::ErrorHandler(const int ledPin, const int buttonPin) :
    ledPin(ledPin),
    buttonPin(buttonPin)
{
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    if (buttonPin != -1)
    {
        pinMode(buttonPin, INPUT_PULLUP);
    }
}

void ErrorHandler::Error(const bool recoverable) const
{
    while (!recoverable || buttonPin == -1 || digitalRead(buttonPin))
    {
        digitalWrite(ledPin, HIGH);
        delay(500);
        digitalWrite(ledPin, LOW);
        delay(500);
    }
}
