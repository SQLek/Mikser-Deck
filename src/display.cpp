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

void testScreen()
{
    oled.clearDisplay();

    // for loop generates gauge level using images 1-26 using i variable
    for (uint8_t i = 1; i <= 26; i++)
    {
        // second loop for generating gauge indicator digits
        for (uint8_t c = 0; c <= 4; c++)
        {
            drawIndicator(c, i);
        }
        oled.display();
        delay(70);
    }
    delay(570);
    oled.clearDisplay();

    for (uint8_t i = 26; i >= 1; i--)
    {
        for (uint8_t c = 0; c <= 4; c++)
        {
            drawIndicator(c, i);
        }
        oled.display();
        oled.clearDisplay();
        delay(70);
    }
}