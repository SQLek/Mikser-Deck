#include <Arduino.h>
#include "avdweb_VirtualDelay.h"

// set potentiometer pin numbers.
const uint8_t analogInputs[] = {4};
const uint8_t numPots = sizeof(analogInputs);

// set button pin numbers.
const uint8_t buttonInputs[] = {47, 48};
const uint8_t numButtons = sizeof(buttonInputs);

// delay timer for buttons
static VirtualDelay *buttonTimers;
static uint8_t *buttonLastValues;
static uint8_t *buttonStates;

void setup()
{
  // set analog input pinmode for each pin assigned to potentiometers
  analogReadResolution(10);
  for (uint8_t i = 0; i < numPots; i++)
  {
    pinMode(analogInputs[i], INPUT);
  }

  // set digtal input pinmode for each pin assigned to buttons
  buttonTimers = new VirtualDelay[numButtons];
  buttonLastValues = new uint8_t[numButtons];
  buttonStates = new uint8_t[numButtons];
  for (uint8_t i = 0; i < numButtons; i++)
  {
    pinMode(buttonInputs[i], INPUT);
    buttonLastValues[i] = 0;
    buttonStates[i] = 0;
  }

  Serial.begin(9600);
}

// read each slider status and send over serial
void readSlidersSendSerial()
{
  for (uint8_t i = 0; i < numPots; i++)
  {
    if (i != 0)
      Serial.print("|");

    Serial.printf("%d", analogRead(analogInputs[i]));
  }
  Serial.println();
}

void readButtonsSendSerial()
{
  if (!numButtons)
    return;

  // read each button status and send data over serial
  Serial.print("but");
  for (uint8_t i = 0; i < numButtons; i++)
  {
    uint8_t lastValue = buttonLastValues[i];
    uint8_t value = digitalRead(buttonInputs[i]);
    
    // check if value has changed if not then skip and send over serial
    if (value == lastValue)
    {
      Serial.printf("|%d", buttonStates[i]);
      continue;
    }
    
    // if debouncing timer not elapsed skip button check and send over serial
    if (buttonTimers[i].running && !buttonTimers[i].elapsed())
    {
      Serial.printf("|%d", buttonStates[i]);
      continue;
    }

    // button status changed, restart timer
    buttonLastValues[i] = value;
    buttonTimers[i].start(50);

    // if pressed toggle button value and send over serial
    if (value)
    {
      buttonStates[i] = ~buttonStates[i] & 0x01;
    }

    Serial.printf("|%d", buttonStates[i]);
  }
  Serial.println();
}

void loop()
{
  readSlidersSendSerial();
  readButtonsSendSerial();
  delay(10);
}
