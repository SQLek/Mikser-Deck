#include "display.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT);

int8_t points[] = {
    0, 17,
    6, 16,
    11, 14,
    15, 9,
    17, 3,
    17, -3,
    15, -9,
    11, -14,
    6, -16,
    1, -17,
    -6, -16,
    -11, -14,
    -15, -9,
    -17, -3,
    -17, 3,
    -15, 9,
    -11, 14,
    -6, 16,
    0, 17, // fake entry - double of 0 index
    6, 16};

static uint8_t animation;

void setupScreen()
{
    Wire.setPins(SCREEN_SDA, SCREEN_SCL);

    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("fail"));
    }
    oled.clearDisplay();
    animation = 0;
}

void drawCircleIndicator(uint8_t x, uint8_t y, uint8_t size, uint16_t volume, char c)
{
    volume = volume / 64;
    oled.fillCircle(x, y, size, WHITE);
    oled.fillCircle(x, y, size / 2, BLACK);

    for (uint8_t i = 0; i < volume; i++)
    {
        uint8_t j = i + 1;
        oled.fillTriangle(
            x, y,
            x + points[j * 2], y + points[j * 2 + 1],
            x + points[j * 2 + 2], y + points[j * 2 + 3],
            BLACK);
    }

    oled.drawCircle(x, y, size + 1, WHITE);
    oled.drawCircle(x, y, size / 2, WHITE);

    oled.fillTriangle(
        x, y,
        x + points[36], y + points[36 + 1] + 1,
        x + points[38], y + points[38 + 1] + 1, // pierwsza linia
        BLACK);

    oled.fillTriangle(
        x, y,
        x + points[36], y + points[36 + 1] + 1,
        x + points[34], y + points[34 + 1] + 1, // druga linia
        BLACK);
    oled.drawLine(
        x + points[38] / 2, y + points[38 + 1] / 2,
        x + points[38] - 1, y + points[38 + 1] - 1,
        WHITE);
    oled.drawLine(
        x + points[34] / 2, y + points[34 + 1] / 2,
        x + points[34] - 1, y + points[34 + 1] - 1,
        WHITE);

    oled.drawChar(x - 2, y - 3, c, WHITE, BLACK, 1);
}

void testScreen()
{
    drawCircleIndicator(24, 47, 16, animation * 64, '3');
    drawCircleIndicator(64, 47, 16, animation * 64, '4');
    drawCircleIndicator(104, 47, 16, animation * 64, '5');
    drawCircleIndicator(44, 17, 16, animation * 64, '1');
    drawCircleIndicator(84, 17, 16, animation * 64, '2');

    oled.display();
    animation++;
    if (animation >= 16)
    {
        animation = 0;
    }

    int16_t cursorX = oled.getCursorX();
    int16_t cursorY = oled.getCursorY();
    Serial.printf("X: %d Y: %d", cursorX, cursorY);
}
