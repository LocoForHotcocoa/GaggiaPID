#pragma once

#include "buttons/buttonHandler.h"
#include "display/display.h"
#include "pid/PIDController.h"
#include "pid/SSR.h"
#include "pid/thermocouple.h"
#include "eeprom/eepromHandler.h"

class GaggiaPID {
public:
    enum Mode {
        BREW_MODE = 0,
        STEAM_MODE
    };

    GaggiaPID(uint8_t tempSwitchPin = TEMP_SWITCH_PIN);
    void begin();
    void menuLoop();

    float brewTemp;
    float steamTemp;
    float currentTemp;
    float kp, ki, kd;
    float ssr_duty;
private:
    Mode checkMode();
    Display m_display;
    ButtonHandler m_buttons;
    Thermocouple m_thermocouple;
    PIDController m_pid;
    SSR m_ssr;
    EEPROMHandler m_eeprom;

    uint8_t m_switchPin;
    static constexpr uint8_t TEMP_SWITCH_PIN = 10; 
};