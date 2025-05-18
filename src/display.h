// display.h
#pragma once

#include <Adafruit_GFX.h> 
#include <gfxfont.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Wire.h>

#include "config.h"

class Display {
public:
    enum PidOption { KP_OPTION, KI_OPTION, KD_OPTION } pidOption;
    enum TempOption { BREW_OPTION, STEAM_OPTION } tempOption;

    Display();
    void initDisplay();
    void brewDisplay(double& currentTemp, uint8_t& brewTemp);
    void steamDisplay(double& currentTemp, uint8_t& steamTemp);
    void pidDisplay(uint8_t& kp, uint8_t& ki, uint8_t& kd, PidOption option);
    void tempDisplay(uint8_t& brewTemp, uint8_t& steamTemp, TempOption option);

private:
    Adafruit_SSD1306 display;
};
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

