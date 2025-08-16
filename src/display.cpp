#include "display.h"

Display::Display() = default;

bool Display::begin(uint8_t screenWidth, uint8_t screenHeight, 
                    uint8_t reset, uint8_t dispAddress) {
    m_display = Adafruit_SSD1306(screenWidth, screenHeight, &Wire, reset);
    m_display.begin(SSD1306_SWITCHCAPVCC, dispAddress);
    m_display.clearDisplay();
    m_display.setTextColor(WHITE);
    return true;
}

// this will be a cute little logo
void Display::initDisplay(char* msg) {
    m_display.clearDisplay();
    m_display.setTextSize(2);
    m_display.setCursor(0,0);
    m_display.println(msg);
    m_display.display();
}

// this will be a cute coffee mug with temp on it!
void Display::brewDisplay(float& currentTemp, float& brewTemp) {
    m_display.clearDisplay();
    m_display.setTextSize(2);
    m_display.setCursor(0,0);

    m_display.print(currentTemp, 1);
    m_display.print(" -> ");
    m_display.print(brewTemp, 1);
    m_display.display();
    screen = BREW_SCREEN;
}

// this will be a cute little steam wand!
void Display::steamDisplay(float& currentTemp, float& steamTemp) {
    m_display.clearDisplay();
    m_display.setTextSize(2);
    m_display.setCursor(0,0);

    m_display.print(currentTemp, 1);
    m_display.print(" -> ");
    m_display.print(steamTemp, 1);
    m_display.display();
    screen = STEAM_SCREEN;
}

void Display::settingsDisplay(float& brewTemp, float& steamTemp,
                              float& kp, float& ki, float& kd) {
    m_display.clearDisplay();
    m_display.setTextSize(1);
    m_display.setCursor(0,0);
    m_display.println("   SETTINGS");

    m_display.print("brew =  ");
    m_display.print(brewTemp, 1);
    m_display.println();

    m_display.print("steam =  ");
    m_display.print(steamTemp, 1);
    m_display.println();

    m_display.print("kp   =  ");
    m_display.print(kp, 1);
    m_display.println();

    m_display.print("ki   =  ");
    m_display.print(ki, 1);
    m_display.println();

    m_display.print("kd   =  ");
    m_display.print(kd, 1);
    m_display.println();

    m_display.setCursor(30, (option+1)*8);
    m_display.print(">");
    m_display.display();
    screen = SETTINGS_SCREEN;
}

void Display::errorDisplay(char* msg)
{
    m_display.clearDisplay();
    m_display.println(msg);
    m_display.display();
}