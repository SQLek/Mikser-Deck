#include <Arduino.h>
#include "avdweb_VirtualDelay.h"

// #define POTS_PINS 4
// #define MUTE_PINS 47,48
// #define MACRO_KEYPAD;
// #define USES_SCREEN;

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
