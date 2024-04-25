#warning "DUPA"
#define POTS_PINS 1
//#define MUTE_PINS 40


//Remember to edit lib TFT_eSPI pins, display type & resolution
#define LIB_eSPI

//uses adafruit SSD1306 lib
//#define SSD1306

#ifdef SSD1306
#define SCREEN_SCL 39
#define SCREEN_SDA 37
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SSD1306 0x3C
#endif

