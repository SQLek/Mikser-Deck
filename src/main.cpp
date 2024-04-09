
#include "main.h"

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

#ifdef SSD1306

  //Wire.setPins(SCREEN_SDA, SCREEN_SCL);
  if (oled.begin(SSD1306_SWITCHCAPVCC, SSD1306))
  {
    Serial.println("SSD1306 init failed!");
  }
  oled.clearDisplay();
  oled.clearDisplay();
#endif

  Serial.begin(9600);
}

// read each slider status and send over serial
#ifdef POTS_ENABLE
void readSlidersSendSerial()
{
  for (uint8_t i = 0; i < numPots; i++)
  {
    if (i != 0)
      Serial.print("|");

    Serial.print(String(analogRead(analogInputs[i])));
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
      Serial.print("|");
      Serial.print(String(buttonStates[i]));
      continue;
    }

    // if debouncing timer not elapsed skip button check and send over serial
    if (buttonTimers[i].running && !buttonTimers[i].elapsed())
    {
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

    Serial.print("|");
    Serial.print(String(buttonStates[i]));
  }
  Serial.println();
}
#endif

void drawBar(int level) {

   int barHeight = oled.height() * level / 100; // Scale the bar height according to `poziom` variable
  oled.clearDisplay();

  for (int i = 0; i < barHeight; i++) {
    oled.drawPixel(0, oled.height() - i - 1, SSD1306_WHITE); // Draw each pixel of the bar
  }

  oled.display();
}

void loop()
{

#ifdef POTS_ENABLE
  readSlidersSendSerial();
#endif

#ifdef MUTE_ENABLE
  readButtonsSendSerial();
#endif
#ifdef SCREEN_SCL
 drawBar(500);
#endif
 
}
  