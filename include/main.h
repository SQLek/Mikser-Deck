#include <Arduino.h>
#include "avdweb_VirtualDelay.h"

//#define POTS_PINS 4
//#define MUTE_PINS 47,48
//#define MACRO_KEYPAD;
//#define USES_SCREEN;

#ifdef POTS_PINS
#define POTS_ENABLE
// set potentiometer pin numbers.
const uint8_t analogInputs[] = {POTS_PINS};
const uint8_t numPots = sizeof(analogInputs);
#endif

#ifdef MUTE_PINS
#define MUTE_ENABLE
// set button pin numbers.
const uint8_t buttonInputs[] = {MUTE_PINS};
const uint8_t numButtons = sizeof(buttonInputs);

// delay timer for buttons
static VirtualDelay *buttonTimers;
static uint8_t *buttonLastValues;
static uint8_t *buttonStates;
#endif

#ifdef SCREEN_SCL
#include <Wire.h>

#ifdef SSD1306
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire, SCREEN_SCL, SCREEN_SDA);

#endif /* SSD1306 */

#endif