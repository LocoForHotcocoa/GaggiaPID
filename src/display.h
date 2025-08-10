// display.h
#pragma once

#include <Adafruit_GFX.h> 
#include <gfxfont.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Wire.h>

#include <stdint.h>


enum Screen { BREW_SCREEN, STEAM_SCREEN, TEMP_SCREEN, PID_SCREEN };

class Display {
public:
    enum Opt { BREW, STEAM, KP, KI, KD };

    Screen screen = BREW_SCREEN;
    Display();
    bool begin(uint8_t screenWidth = SCREEN_WIDTH, 
               uint8_t screenHeight = SCREEN_HEIGHT, 
               uint8_t reset = OLED_RESET, 
               uint8_t dispAddress = DISP_ADDRESS);
               
    void initDisplay();
    void brewDisplay(float& currentTemp, float& brewTemp);
    void steamDisplay(float& currentTemp, float& steamTemp);
    void settingsDisplay(float& brewTemp, float& steamTemp,
                         float& kp, float& ki, float& kd, 
                         uint8_t option = Opt::BREW);
    void errorDisplay(char* msg);

    static constexpr uint8_t SCREEN_WIDTH  = 128;
    static constexpr uint8_t SCREEN_HEIGHT = 64;
    static constexpr uint8_t OLED_RESET    = 4;
    static constexpr uint8_t DISP_ADDRESS  = 0x3C;
private:
    Adafruit_SSD1306 m_display;
};
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

