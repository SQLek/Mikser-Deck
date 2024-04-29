#include "display.h"

#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

uint16_t indicatorValues[] = {
    0,
    0,
    0,
    0,
    0,
};

bool indicatorMute[] = {
    false,
    false,
    false,
    false,
    false,
};

void setupScreen()
{
    tft.init();
    tft.setRotation(7);
    tft.fillScreen(TFT_BLACK);
}

void drawIndicator(uint8_t indicatorId)
{
    uint32_t angle = (uint32_t)indicatorValues[indicatorId] * 320 / 1024 + 20;
    Serial.printf("angle %d\n", angle);
    // tft.fillCircle(60, 50, 60, TFT_DARKGREEN);
    // tft.drawSmoothCircle(60, 50, 60, TFT_SILVER, TFT_BLUE);
    //tft.drawArc(60, 50, 55, 35, 17, 343, TFT_RED, TFT_BLACK, true);
    tft.drawArc(60, 50, 50, 40, angle, 340, TFT_PINK, TFT_RED,true);
    //sleep(10);
}

void drawScreen()
{
    tft.fillScreen(TFT_NAVY);
    //for (uint8_t c = 0; c <= 4; c++)
    //{
        drawIndicator(0);
    //}
}

void setIndicatorValue(uint8_t indicatorId, uint16_t value)
{
#ifdef LIB_eSPI
    if (indicatorId < sizeof(indicatorValues) / sizeof(uint16_t))
        indicatorValues[indicatorId] = value;
#endif
}

void setIndicatorMute(uint8_t indicatorId, bool mute)
{
#ifdef LIB_eSPI
    if (indicatorId < sizeof(mute) / sizeof(bool))
        indicatorMute[indicatorId] = mute;
#endif
}
