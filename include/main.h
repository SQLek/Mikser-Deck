#include <Arduino.h>
#include "avdweb_VirtualDelay.h"

#define POTENTIOMETERS
//#define USES_MUTE_BUTTONS
//#define MACRO_KEYPAD;
//#define USES_SCREEN;

#ifdef POTENTIOMETERS
// set potentiometer pin numbers.
const uint8_t analogInputs[] = {A0};
const uint8_t numPots = sizeof(analogInputs);
#endif

#ifdef USES_MUTE_BUTTONS
// set button pin numbers.
const uint8_t buttonInputs[] = {47, 48};
const uint8_t numButtons = sizeof(buttonInputs);

// delay timer for buttons
static VirtualDelay *buttonTimers;
static uint8_t *buttonLastValues;
static uint8_t *buttonStates;
#endif
