
#include "main.h"
#include "display.h"

void setup()
{
#ifdef POTS_ENABLE
// set analog input pinmode for each pin assigned to potentiometers
#ifndef ARDUINO_ARCH_AVR
  analogReadResolution(10);
#endif
  for (uint8_t i = 0; i < numPots; i++)
  {
    pinMode(analogInputs[i], INPUT);
  }
#endif

#ifdef MUTE_ENABLE
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
#endif

  Serial.begin(9600);

#ifdef LIB_eSPI
  setupScreen();
#endif
}

// read each slider status and send over serial
#ifdef POTS_ENABLE
void readSlidersSendSerial()
{
  for (uint8_t i = 0; i < numPots; i++)
  {
    if (i != 0)
      Serial.print("|");

    uint16_t value = analogRead(analogInputs[i]);
    setIndicatorValue(i, value);
    Serial.print(String(value));
  }
  Serial.println();
}
#endif

#ifdef MUTE_ENABLE
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
      setIndicatorMute(i, buttonStates[i]);
      Serial.print("|");
      Serial.print(String(buttonStates[i]));
      continue;
    }

    // if debouncing timer not elapsed skip button check and send over serial
    if (buttonTimers[i].running && !buttonTimers[i].elapsed())
    {
      setIndicatorMute(i, buttonStates[i]);
      Serial.print("|");
      Serial.print(String(buttonStates[i]));
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

    setIndicatorMute(i, buttonStates[i]);
    Serial.print("|");
    Serial.print(String(buttonStates[i]));
  }
  Serial.println();
}
#endif

void loop()
{

#ifdef POTS_ENABLE
  readSlidersSendSerial();
#endif

#ifdef MUTE_ENABLE
  readButtonsSendSerial();
#endif

#ifdef LIB_eSPI
  drawScreen();
#endif
}
