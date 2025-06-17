#include "display.h"

Display::Display(uint8_t screenWidth, uint8_t screenHeight, 
                 uint8_t reset, uint8_t dispAddress) 
{
    display = Adafruit_SSD1306(screenWidth, screenHeight, &Wire, reset);
    display.begin(SSD1306_SWITCHCAPVCC, dispAddress);
    display.clearDisplay();
    display.setTextColor(WHITE);
}

// this will be a cute little coffee mug with temperature on it!
void Display::initDisplay() 
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("Coffee <3");
    display.display();
}

void Display::brewDisplay(double& currentTemp, uint8_t brewTemp) 
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println(String(currentTemp) + "->" + String(brewTemp));
    display.display();
    screen = BREW_SCREEN;
}

// this will be a cute little steam wand!
void Display::steamDisplay(double& currentTemp, uint8_t steamTemp) 
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("Temp: " + String(currentTemp) + "->" + String(steamTemp));
    display.display();
    screen = STEAM_SCREEN;
}

void Display::pidDisplay(uint8_t kp, uint8_t ki, uint8_t kd) 
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("   PID");
    display.println("kp  =  " + String(kp));
    display.println("ki  =  " + String(ki));
    display.println("kd  =  " + String(kd));
    display.setCursor(60, (pidOption+1)*16);
    display.print(">");
    display.display();
    screen = PID_SCREEN;
}

void Display::tempDisplay(uint8_t brewTemp, uint8_t steamTemp) 
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("   Temperature");
    display.println("Brew  =  " + String(brewTemp));
    display.println("Steam =  " + String(steamTemp));
    display.setCursor(84, (tempOption+1)*16);
    display.print(">");
    display.display();
    screen = TEMP_SCREEN;
}

void Display::errorDisplay(char* msg)
{
    display.clearDisplay();
    display.println(msg);
    display.display();
}