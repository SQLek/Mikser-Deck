#include "display.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT);

int8_t points[] = {
    0, 16,
    6, 15,
    11, 13,
    14, 8,
    16, 3,
    16, -3,
    14, -8,
    11, -13,
    -6, -15,
    -6, -16,
    -11, -15,
    -14, -13,
    -16, -8,
    -16, -3,
    -14, 3,
    -11, 8,
    -6, 13,
    0, 15,
    0, 16, // fake entry - double of 0 index
};

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
    oled.fillCircle(x, y, size, WHITE);
    oled.fillCircle(x, y, size/2, BLACK);

    oled.fillTriangle(
        x,y,
        x+points[animation],y+points[animation+1],
        x+points[animation+2],y+points[animation+3],
        BLACK
    );

    oled.drawChar(x-2, y-3, c, WHITE, BLACK, 1);
}

void testScreen()
{
    drawCircleIndicator(24,47,16,512,'3');
    drawCircleIndicator(64,47,16,512,'4');
    drawCircleIndicator(104,47,16,512,'5');
    drawCircleIndicator(44,17,16,512,'1');
    drawCircleIndicator(84,17,16,512,'2');
    
    oled.display();
    animation += 2;
    if (animation >= 36) {
        animation = 0;
    }

    int16_t cursorX = oled.getCursorX();
    int16_t cursorY = oled.getCursorY();
    Serial.printf("X: %d Y: %d", cursorX, cursorY);
}
