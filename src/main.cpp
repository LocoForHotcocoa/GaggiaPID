#include <Arduino.h>
#include <Adafruit_MCP9601.h>
#include <EEPROM.h>

#include "config.h"
#include "display.h"
#include "input.h"

Adafruit_MCP9601 mcp; //setup for MCP9601
Display display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_RESET, DISP_ADDRESS);
Input input(UP_PIN, DOWN_PIN, SELECT_PIN, BACK_PIN, INPUT_INTERVAL_MS);

uint8_t brewTemp, steamTemp;
uint8_t kp, ki, kd;

double currentTemp;  // will be fed from TC
double output;       // for PWM control
double targetTemp;  // will be fed into PID algorithm

uint32_t windowStartTime;

void setup()
{
    // put your setup code here, to run once:
    // Check if EEPROM is initialized
    // I use a initialization flag at 0x0 to assure that the EEPROM has saved values
    // if flag address is empty, then we need to use the defaults and save them to EEPROM (defined above)
    if (EEPROM.read(FLAG_ADDR) != INIT_FLAG) {
        // EEPROM is not initialized, so write the default values
        EEPROM.write(BREW_ADDR, BREWTEMP_DEFAULT);
        EEPROM.write(STEAM_ADDR, STEAMTEMP_DEFAULT);
        EEPROM.write(KP_ADDR, KP_DEFAULT);
        EEPROM.write(KI_ADDR, KI_DEFAULT);
        EEPROM.write(KD_ADDR, KD_DEFAULT);
        EEPROM.write(FLAG_ADDR, INIT_FLAG);  // Set the flag to indicate initialization
    }

    // Now that we are sure EEPROM has pid values, we read the values from EEPROM into our program
    brewTemp = EEPROM.read(BREW_ADDR);
    steamTemp = EEPROM.read(STEAM_ADDR);
    kp = EEPROM.read(KP_ADDR);
    ki = EEPROM.read(KI_ADDR);
    kd = EEPROM.read(KD_ADDR);

    if (! mcp.begin(TC_ADDRESS)) {
        display.errorDisplay("TC ERROR");
        while(1);
    }
    mcp.setADCresolution(MCP9600_ADCRESOLUTION_18);
    mcp.setThermocoupleType(MCP9600_TYPE_K);
    mcp.setFilterCoefficient(3);
    mcp.enable(true);

    display.initDisplay();
    delay(2000);

    windowStartTime = millis();
    display.brewDisplay(currentTemp, brewTemp);
}

void checkButtons() 
{
    input.update();

    switch (display.screen) {
        case BREW_SCREEN:
            if(input.select.fell()) 
                display.tempDisplay(brewTemp, steamTemp);
            else if(input.back.fell())
                display.pidDisplay(kp, ki, kd);
            break;
        case STEAM_SCREEN:
            if(input.select.fell())
                display.tempDisplay(brewTemp, steamTemp);
            else if(input.back.fell())
                display.pidDisplay(kp, ki, kd);
        
        case TEMP_SCREEN:
            if(input.select.fell()) {
                switch (display.tempOption) {
                    case BREW_OPTION: display.tempOption = STEAM_OPTION;
                    case STEAM_OPTION: display.tempOption = BREW_OPTION;
                }
                display.tempDisplay(brewTemp, steamTemp);
            }
            else if(input.up.fell()) {
                switch (display.tempOption) {
                    case BREW_OPTION:
                        brewTemp++;
                        EEPROM.write(BREW_ADDR, brewTemp);
                        break;
                    case STEAM_OPTION:
                        steamTemp++;
                        EEPROM.write(STEAM_ADDR, steamTemp);
                        break;
                }
                display.tempDisplay(brewTemp, steamTemp);
            }
            else if(input.down.fell()) {
                                switch (display.tempOption) {
                    case BREW_OPTION:
                        brewTemp--;
                        EEPROM.write(BREW_ADDR, brewTemp);
                        break;
                    case STEAM_OPTION:
                        steamTemp--;
                        EEPROM.write(STEAM_ADDR, steamTemp);
                        break;
                }
                display.tempDisplay(brewTemp, steamTemp);
            }
            else if(input.back.fell()) {
                display.brewDisplay(currentTemp, brewTemp);
            }
        case PID_SCREEN:
            if (input.select.fell()) {
                switch (display.pidOption) {
                    case KP_OPTION: display.pidOption = KI_OPTION; break; // Switch to Ki setting
                    case KI_OPTION: display.pidOption = KD_OPTION; break; // Switch to Kd setting
                    case KD_OPTION: display.pidOption = KP_OPTION;        // Switch to Kp setting
                }
                display.pidDisplay(kp, ki, kd);
            }
            else if (input.up.fell()) {
                switch (display.pidOption) {
                    case KP_OPTION: 
                        kp++;
                        EEPROM.write(KP_ADDR, kp);
                        break;
                    case KI_OPTION: 
                        ki++;
                        EEPROM.write(KI_ADDR, ki);
                        break;
                    case KD_OPTION:
                        kd++;
                        EEPROM.write(KD_ADDR, kd);
                }
                // myPID.SetTunings(kp, ki, kd);
                display.pidDisplay(kp, ki, kd);
            }
            else if (input.select.fell()) {
                switch (display.pidOption) {
                    case KP_OPTION: 
                        kp--; 
                        EEPROM.write(KP_ADDR, kp);
                        break;
                    case KI_OPTION: 
                        ki--;
                        EEPROM.write(KI_ADDR, ki);
                        break;
                    case KD_OPTION: 
                        kd--;
                        EEPROM.write(KD_ADDR, kd);
                }
                // myPID.SetTunings(kp, ki, kd);
                display.pidDisplay(kp, ki, kd);
            }
            else if (input.back.fell()) {
                display.brewDisplay(currentTemp, brewTemp);
            }
    }
}

void loop() 
{
    checkButtons();

    // update everything every 500 ms
    // this is some goofy logic that I got from the PID_v2 arduino docs that's supposed to work with a digital output (instead of PWM). I hope it works!
    if ( millis() - windowStartTime > PID_WINDOW_SIZE) {
        currentTemp = mcp.readThermocouple();
        // myPID.Compute();
        windowStartTime += PID_WINDOW_SIZE;

        if (output < millis() - windowStartTime)
            digitalWrite(RELAY_PIN, HIGH);
        else
            digitalWrite(RELAY_PIN, LOW);
    }
}