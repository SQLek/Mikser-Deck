#include "display.h"
#include "main.h"

#include <TFT_eSPI.h>
#include <SPI.h>
#include <FastTrig.h>
#include "speaker.h"
#include "microphone.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

uint16_t indicatorValues[numPots];
uint8_t indicatorMute[numButtons];

void setupScreen()
{
    tft.init();
    tft.setRotation(3);
    sprite.setTextSize(4);
    sprite.setSwapBytes (true);
    sprite.createSprite(TFT_HEIGHT, TFT_WIDTH, 1);
    for (int i = 0; i < numButtons; i++)
    {
        indicatorMute[i] = 0;
    }
}

void drawFlowerIndicator(uint16_t x, uint16_t y, uint8_t indicatorId)
{
    int angleValue = map(indicatorValues[indicatorId], 0, 1023, -160, 160);
    for (int angle = -160; angle <= 160; angle += 20)
    {
        float sx = icos((angle - 90)); // * 0.0174532925);
        float sy = isin((angle - 90)); // * 0.0174532925);

        float xp1 = sx * 26 + x;
        float yp1 = sy * 26 + y;

        float xp2 = sx * 40 + x;
        float yp2 = sy * 40 + y;

        byte red = 127;
        byte green = 127;
        byte blue = 127;

        if (angle == 160)
        {
            red = 255;
            green = 0;
            blue = 0;
        }
        if (angle < 160 && angle > 0)
        {
            red = map(angle, 160, 0, 255, 0);
            green = map(angle, 160, 0, 0, 255);
            blue = 0;
        }
        if (angle == 0)
        {
            red = 0;
            green = 255;
            blue = 0;
        }
        if (angle > -160 && angle < 0)
        {
            red = 0;
            green = map(angle, -160, 0, 0, 255);
            blue = map(angle, -160, 0, 255, 0);
        }
        if (angle == -160)
        {
            red = 0;
            green = 0;
            blue = 255;
        }

        if (angle > angleValue)
        {
            red = 63;
            green = 63;
            blue = 63;
        }
        uint16_t colour = sprite.color565(red, green, blue);
        sprite.drawWedgeLine(xp1, yp1, xp2, yp2, 2, 5, colour, TFT_BLACK);
    }
}

void drawIndicator(uint16_t x, uint16_t y, uint8_t indicatorId)
{
    uint32_t angle = (uint32_t)indicatorValues[indicatorId] * 320 / 1024 + 20;

    uint8_t red = (angle <= 180) ? map(angle, 20, 180, 0, 255) : 255;
    uint8_t green = (angle >= 160 && angle <= 340) ? map(angle, 160, 340, 255, 0) : (angle < 160) ? 255
                                                                                                  : 0;
    uint8_t blue = 0;
    uint16_t color = tft.color565(red, green, blue); // Tworzenie gradientu kolorów

    sprite.fillCircle(x, y, 43, TFT_DARKGREY);
    sprite.drawArc(x, y, 43, 25, 20, 340, TFT_BLACK, TFT_TRANSPARENT, true);
    sprite.drawArc(x, y, 43, 25, 20, angle, color, TFT_TRANSPARENT, true);
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

void drawMuteIndicator(uint16_t x, uint16_t y, uint8_t indicatorId)
{
    if (indicatorMute[indicatorId] == 0)
        return;
    sprite.drawWideLine(x - 35, y - 35, x + 35, y + 35, 8, TFT_RED, TFT_BLACK);
    sprite.drawWideLine(x - 35, y - 35, x + 35, y + 35, 2, 0xea49, 0xe946);
    sprite.drawWideLine(x + 35, y - 35, x - 35, y + 35, 8, TFT_RED, TFT_BLACK);
    sprite.drawWideLine(x + 35, y - 35, x - 35, y + 35, 2, 0xea49, 0xe946);
}

void drawScreen()
{
    sprite.fillSprite(TFT_BLACK);
    for (uint8_t c = 0; c <= 4; c++)
    {
        // uint8_t c = 0;
        uint16_t x = 50 + c * 51; // X position of meters first value is base and second value is for next meters
        uint16_t y = 79 + 44;     // y position of meters 44
        if (c % 2 == 1)           // if value is odd then move down
            y -= 79;              // y position of bottom meters +=79

#ifdef SMOOTH_INDICATOR
        drawIndicator(x, y, c);
#endif
#ifdef FLOWER_INDICATOR
        drawFlowerIndicator(x, y, c);
#endif
        drawMuteIndicator(x, y, c);
    }
    sprite.pushImage(5,5,43,66, image_data_speaker);
    sprite.pushImage(320-65,5,43,65, image_data_microphone);
    sprite.pushSprite(0, 0, TFT_BLACK);
}


void setIndicatorValue(uint8_t indicatorId, uint16_t value)
{
#ifdef LIB_eSPI
    if (indicatorId < sizeof(indicatorValues) / sizeof(uint16_t))
    {
        indicatorValues[indicatorId] = value;
    }
#endif
}

void setIndicatorMute(uint8_t indicatorId, uint8_t mute)
{
#ifdef LIB_eSPI
    if (indicatorId < sizeof(indicatorMute) / sizeof(uint8_t))
    {
        indicatorMute[indicatorId] = mute;
    }
#endif
}
