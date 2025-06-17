// display.h
#pragma once

#include <Adafruit_GFX.h> 
#include <gfxfont.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Wire.h>

#include <stdint.h>

enum PidOption { KP_OPTION, KI_OPTION, KD_OPTION };
enum TempOption { BREW_OPTION, STEAM_OPTION };
enum Screen { BREW_SCREEN, STEAM_SCREEN, TEMP_SCREEN, PID_SCREEN };

class Display 
{
public:
    PidOption pidOption = KP_OPTION;
    TempOption tempOption = BREW_OPTION;
    Screen screen = BREW_SCREEN;
    Display::Display(uint8_t screenWidth, uint8_t screenHeight, 
                 uint8_t reset, uint8_t dispAddress);
    
    void initDisplay();
    void brewDisplay(double& currentTemp, uint8_t brewTemp);
    void steamDisplay(double& currentTemp, uint8_t steamTemp);
    void pidDisplay(uint8_t kp, uint8_t ki, uint8_t kd);
    void tempDisplay(uint8_t brewTemp, uint8_t steamTemp);

    void errorDisplay(char* msg);
private:
    Adafruit_SSD1306 display;
};
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

