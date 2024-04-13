#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <Arduino.h>

void setIndicatorValue(uint8_t indicatorId, uint16_t value);
void setIndicatorMute(uint8_t indicatorId, bool mute);

#ifdef SCREEN_SCL

void setupScreen();
void drawScreen();

#endif /* SCREEN_SCL */

#endif /* __DISPLAY_H */
