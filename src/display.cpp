#include "display.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT);

void setupScreen()
{
    Wire.setPins(SCREEN_SDA, SCREEN_SCL);

    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("fail"));
    }
    oled.clearDisplay();
}

void testScreen()
{
    oled.setTextColor(INVERSE);
    oled.setTextSize(1);
    oled.setCursor(45, 20);
    oled.println("MEOW");
    oled.display();
    Serial.println("UwU");

    oled.drawFastVLine(50, 50, 30, INVERSE);

    int16_t cursorX = oled.getCursorX();
    int16_t cursorY = oled.getCursorY();
    Serial.printf("X: %d Y: %d", cursorX, cursorY);
}
