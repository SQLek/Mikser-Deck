#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <Arduino.h>

void setIndicatorValue(uint8_t indicatorId, uint16_t value);
void setIndicatorMute(uint8_t indicatorId, bool mute);

#ifdef LIB_eSPI

void setupScreen();
void drawScreen();

#endif /* LIB_eSPI */

#endif /* __DISPLAY_H */
