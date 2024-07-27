#include "display.h"
#include "main.h"

#include <TFT_eSPI.h>
#include <SPI.h>
#include "speaker.h"
#include "microphone.h"
#include "back.h"
#include "button.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite baseSprite = TFT_eSprite(&tft);
TFT_eSprite background = TFT_eSprite(&tft);
TFT_eSprite sprite = TFT_eSprite(&tft);
TFT_eSprite imgSprite = TFT_eSprite(&tft);
TFT_eSprite muteSprite = TFT_eSprite(&tft);

uint16_t indicatorValues[numPots];
uint8_t indicatorMute[numButtons];
const float radianFactor = 0.0174532925;
static float precalcSin[17];
static float precalcCos[17];

uint8_t lastPetalIds[numPots];

void setupScreen()
{
    for (int i = 0; i < numPots; i++)
    {
        lastPetalIds[i] = -1;
    }

    tft.init();
    tft.setRotation(7);
    tft.fillScreen(TFT_BLACK);
    sprite.setTextSize(4);

    pinMode(3, OUTPUT);
    analogWrite(3, 120);

    imgSprite.setSwapBytes(true);
    background.setSwapBytes(true);

    sprite.createSprite(TFT_HEIGHT, TFT_WIDTH);
    imgSprite.createSprite(TFT_HEIGHT, TFT_WIDTH);
    muteSprite.createSprite(TFT_HEIGHT, TFT_WIDTH);
    background.createSprite(TFT_HEIGHT, TFT_WIDTH);
    baseSprite.createSprite(TFT_HEIGHT, TFT_WIDTH);

    for (int i = 0; i < numButtons; i++)
    {
        indicatorMute[i] = 0;
    }

    // precalc sin and cos to save cpu cycles
    for (int i = 0; i <= 16; ++i)
    {
        int angle = -160 + i * 20;
        precalcSin[i] = sin((angle - 90) * radianFactor);
        precalcCos[i] = cos((angle - 90) * radianFactor);
    }

    background.pushImage(0, 0, 320, 170, image_data_back);
}

void drawFlowerIndicator(uint16_t x, uint16_t y, uint8_t indicatorId)
{
    int petalId = map(indicatorValues[indicatorId], 0, 1010, 0, 17);
    if (petalId == lastPetalIds[indicatorId])
        return;

    uint16_t lastDrawnColour = sprite.color565(63, 63, 63);
    for (int i = 0; i <= 16; ++i)
    {
        int angle = -160 + i * 20;
        float sx = precalcCos[i];
        float sy = precalcSin[i];

        float xp1 = sx * 26 + x;
        float yp1 = sy * 26 + y;

        float xp2 = sx * 40 + x;
        float yp2 = sy * 40 + y;

        byte red = 0, green = 0, blue = 0;

        if (angle == -160)
        {
            blue = 255;
        }
        else if (angle < 0)
        {
            green = map(angle, -160, 0, 0, 255);
            blue = map(angle, -160, 0, 255, 0);
        }
        else if (angle == 0)
        {
            green = 255;
        }
        else if (angle > 0)
        {
            red = map(angle, 0, 160, 0, 255);
            green = map(angle, 0, 160, 255, 0);
        }
        else if (angle == 160)
        {
            red = 255;
        }

        if (i >= petalId)
        {
            red = 63;
            green = 63;
            blue = 63;
        }
        else
        {
            lastDrawnColour = sprite.color565(red, green, blue);
        }

        uint16_t colour = sprite.color565(red, green, blue);
        sprite.drawWedgeLine(xp1, yp1, xp2, yp2, 2, 5, colour, TFT_BLACK);
    }

    sprite.setTextColor(lastDrawnColour);
    sprite.drawChar(0x31 + indicatorId - 1, x - 11, y - 14);
    lastPetalIds[indicatorId] = petalId;
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
    {
        return;
    }
    muteSprite.drawWideLine(x - 35, y - 30, x + 35, y + 30, 8, TFT_RED, TFT_BLACK);
    muteSprite.drawWideLine(x - 35, y - 30, x + 35, y + 30, 2, 0xea49, 0xe946);
    muteSprite.drawWideLine(x + 35, y - 30, x - 35, y + 30, 8, TFT_RED, TFT_BLACK);
    muteSprite.drawWideLine(x + 35, y - 30, x - 35, y + 30, 2, 0xea49, 0xe946);
}

void drawMuteSlimIndicator(uint16_t x, uint16_t y, uint8_t indicatorId)
{
    if (indicatorMute[indicatorId] == 0)
    {
        return;
    }
    muteSprite.drawWideLine(x - 15, y - 29, x + 25, y + 30, 8, TFT_RED, TFT_BLACK);
    muteSprite.drawWideLine(x - 15, y - 29, x + 25, y + 30, 2, 0xea49, 0xe946);
    muteSprite.drawWideLine(x + 25, y - 29, x - 15, y + 30, 8, TFT_RED, TFT_BLACK);
    muteSprite.drawWideLine(x + 25, y - 29, x - 15, y + 30, 2, 0xea49, 0xe946);
}

void drawScreen()
{
    baseSprite.fillSprite(TFT_BLACK);

    muteSprite.fillSprite(TFT_BLACK);

    for (uint8_t c = 1; c <= 5; c++)
    {
        // uint8_t c = 0;
        uint16_t x = 58 + (c - 1) * 51; // X position of meters first value is base and second value is for next meters
        uint16_t y = 79 + 46;           // y position of meters 44
        if (c % 2 == 0)                 // if value is odd then move down
            y -= 79;                    // y position of bottom meters +=79

#ifdef SMOOTH_INDICATOR
        drawIndicator(x, y, c);
#endif
#ifdef FLOWER_INDICATOR
        drawFlowerIndicator(x, y, c);
#endif
        drawMuteIndicator(x, y, c);

        // #ifdef MUTE_ENABLE
        //         readButtonsSendSerial();
        // #endif
    }
    imgSprite.pushImage(10, 5, 43, 66, image_data_speaker);
    drawMuteSlimIndicator(26, 35, 0);
    imgSprite.pushImage(320 - 55, 5, 43, 65, image_data_microphone);
    drawMuteSlimIndicator(320 - 40, 38, numButtons - 1);

    background.pushToSprite(&baseSprite, 0, 0, TFT_BLACK);
    imgSprite.pushToSprite(&baseSprite, 0, 0, TFT_BLACK);

    sprite.pushToSprite(&baseSprite, 0, 0, TFT_BLACK);
    muteSprite.pushToSprite(&baseSprite, 0, 0, TFT_BLACK);
    baseSprite.pushSprite(0, 0);
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
