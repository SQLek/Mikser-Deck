#ifndef __MAIN_H // header guard
#define __MAIN_H

#include <Arduino.h>
#include "avdweb_VirtualDelay.h"

#ifndef POTS_PINS
#define POTS_PINS 2, 4, 6, 8, 10, 13 // default pins (required for error check in VSCode as it doesnt find config.h injected through platformio.ini)
#endif

#ifdef POTS_PINS
#define POTS_ENABLE
// set potentiometer pin numbers.
const uint8_t analogInputs[] = {POTS_PINS};
const uint8_t numPots = sizeof(analogInputs);
#endif

#ifndef MAX_POT_VALUES
#define MAX_POT_VALUES 1023 // default values for pots (required for error check in VSCode as it doesnt find config.h injected through platformio.ini)
#endif

#ifndef MIN_POT_VALUES
#define MIN_POT_VALUES 0 // default values for pots (required for error check in VSCode as it doesnt find config.h injected through platformio.ini)
#endif

#ifndef MUTE_PINS
#define MUTE_PINS 1, 14, 5, 7, 9, 11, 12 // default pins (required for error check in VSCode as it doesnt find config.h injected through platformio.ini)
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

#endif /* __MAIN_H */
