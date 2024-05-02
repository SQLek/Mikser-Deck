#include "display.h"

#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

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
    sprite.setTextSize(4);
    sprite.createSprite(TFT_HEIGHT, TFT_WIDTH, 1);
}

void drawIndicator(uint16_t x, uint16_t y, uint8_t indicatorId)
{
    uint32_t angle = (uint32_t)indicatorValues[indicatorId] * 320 / 1024 + 20;

    sprite.fillCircle(x, y, 43, TFT_DARKGREY);
    sprite.drawArc(x, y, 43, 25, 20, 340, TFT_BLACK, TFT_TRANSPARENT, true);
    sprite.drawArc(x, y, 43, 25, 20, angle, TFT_ORANGE, TFT_TRANSPARENT, true);
    sprite.drawCircle(x, y, 46, TFT_DARKGREY);
    sprite.drawCircle(x, y, 45, TFT_LIGHTGREY);
    sprite.drawCircle(x, y, 44, TFT_WHITE);
    sprite.fillCircle(x, y, 25, TFT_DARKGREY);
    sprite.drawCircle(x, y, 24, TFT_LIGHTGREY);
    sprite.drawCircle(x, y, 23, TFT_BLACK);
    sprite.setTextColor(TFT_BLACK);
    sprite.drawChar(0x31 + indicatorId, x - 7, y - 10);
    sprite.drawChar(0x31 + indicatorId, x - 9, y - 11);
    sprite.drawChar(0x31 + indicatorId, x - 10, y - 12);
    sprite.drawChar(0x31 + indicatorId, x - 11, y - 13);
    sprite.setTextColor(TFT_WHITE);
    sprite.drawChar(0x31 + indicatorId, x - 11, y - 14);
}

void drawScreen()
{
    sprite.fillSprite(TFT_BLACK);
    for (uint8_t c = 0; c <= 4; c++)
    {
        uint16_t x = 50 + c * 51; // X position of meters first value is base and second value is for next meters
        uint16_t y = 44;          // y position of meters
        if (c % 2 == 1)           // if value is odd then move down
            y += 79;              // y position of bottom meters
        drawIndicator(x, y, c);
    }
    sprite.pushSprite(0, 0);
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
