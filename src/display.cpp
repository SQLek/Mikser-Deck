#include "display.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "gfx.h"
#include <Wire.h>
#include <SPI.h>

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT);

// gauge digit indicators 1-5
unsigned char indicatorCharacter[] = {
    0x31,
    0x32,
    0x33,
    0x34,
    0x35,
};

// x position of gauge images
uint8_t indicatorXpos[] = {
    28,
    68,
    8,
    48,
    88,
};

// y position of gauge images
uint8_t indicatorYpos[] = {
    1,
    1,
    31,
    31,
    31,
};

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
    Wire.setPins(SCREEN_SDA, SCREEN_SCL);

    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("fail"));
    }
    oled.clearDisplay();
}

void drawIndicator(uint8_t indicatorId, uint8_t frameId)
{
    oled.drawChar(
        indicatorXpos[indicatorId] + 14,
        indicatorYpos[indicatorId] + 13,
        indicatorCharacter[indicatorId],
        WHITE,
        BLACK,
        1);
    oled.drawBitmap(
        indicatorXpos[indicatorId],
        indicatorYpos[indicatorId],
        epd_bitmap_allArray[frameId],
        32,
        32,
        WHITE);
}

void drawScreen()
{
    oled.clearDisplay();
    for (uint8_t c = 0; c <= 4; c++) {

        uint8_t frameId = indicatorValues[c] / 40 + 1;
        if (frameId < 1)
            frameId = 1;
        if (frameId > epd_bitmap_allArray_LEN - 1)
            frameId = epd_bitmap_allArray_LEN - 1;
        if (indicatorMute[c])
            frameId = 0;

        drawIndicator(c, frameId);
    }
    oled.display();
}

void setIndicatorValue(uint8_t indicatorId, uint16_t value)
{
#ifdef SCREEN_SCL
    if (indicatorId < sizeof(indicatorValues) / sizeof(uint16_t))
        indicatorValues[indicatorId] = value;
#endif
}

void setIndicatorMute(uint8_t indicatorId, bool mute)
{
    #ifdef SCREEN_SCL
    if (indicatorId < sizeof(mute) / sizeof(bool))
        indicatorMute[indicatorId] = mute;
#endif
}
