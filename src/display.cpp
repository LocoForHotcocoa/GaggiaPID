#include "display.h"

Display::Display() {
    display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    display.begin(SSD1306_SWITCHCAPVCC, disp_address);
    display.clearDisplay();
    display.setTextColor(WHITE);
}
// this will be a cute little coffee mug with temperature on it!
void Display::initDisplay() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("Coffee <3");
    display.display();
}
void Display::brewDisplay(double& currentTemp, uint8_t& brewTemp) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println(String(currentTemp) + "->" + String(brewTemp));
    display.display();
}
  
// this will be a cute little steam wand!
void Display::steamDisplay(double& currentTemp, uint8_t& steamTemp) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("Temp: " + String(currentTemp) + "->" + String(steamTemp));
    display.display();
}

void Display::pidDisplay(uint8_t& kp, uint8_t& ki, uint8_t& kd, PidOption option) {
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
}

void Display::tempDisplay(uint8_t& brewTemp, uint8_t& steamTemp, TempOption option) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("   Temperature");
    display.println("Brew  =  " + String(brewTemp));
    display.println("Steam =  " + String(steamTemp));
    display.setCursor(84, (tempOption+1)*16);
    display.print(">");
    display.display();
}