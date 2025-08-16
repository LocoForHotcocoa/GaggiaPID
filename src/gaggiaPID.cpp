#include "gaggiaPID.h"

GaggiaPID::GaggiaPID(uint8_t tempSwitchPin)
    : m_switchPin(tempSwitchPin) {
        pinMode(m_switchPin, INPUT_PULLUP);
    }

void GaggiaPID::begin() {

    m_display.begin();

    if (! m_thermocouple.begin()) {
        m_display.errorDisplay((char*)"TC ERROR");
        while(1);
    }

    char* initMsg;
    if(! m_eeprom.readSettings()) {
        initMsg = (char*)"EEPROM ERR: loading in defaults";
    }
    else {
        initMsg = (char*)"Coffee <3";
    }
    

    brewTemp = m_eeprom.settings.brewTemp;
    steamTemp = m_eeprom.settings.steamTemp;
    kp = m_eeprom.settings.kp;
    ki = m_eeprom.settings.ki;
    kd = m_eeprom.settings.kd;

    if (! m_pid.begin(kp, ki, kd)) {
        m_display.errorDisplay((char*)"PID ERR");
        while(1);
    }

    currentTemp = m_thermocouple.readCelsius();

    m_display.initDisplay(initMsg);
    delay(1000);

    m_pid.begin(kp, ki, kd);
    m_display.brewDisplay(currentTemp, brewTemp);
}

void GaggiaPID::menuLoop() {
    m_buttons.update();
    ButtonHandler::Button pressed = m_buttons.getPressed();
    Mode current_mode = checkMode();
    currentTemp = m_thermocouple.readCelsius();
    if (current_mode == BREW_MODE) 
        ssr_duty = m_pid.update(brewTemp, currentTemp);
    else 
        ssr_duty = m_pid.update(steamTemp, currentTemp);
    m_ssr.setDuty(ssr_duty);
    m_ssr.update();
    
    switch (m_display.screen) {
        case Display::BREW_SCREEN:
        case Display::STEAM_SCREEN:
            if (pressed == ButtonHandler::SELECT) // this takes priority over updating brew/steam display
                m_display.settingsDisplay(brewTemp, steamTemp, kp, ki, kd);
            // only select button is used currently (to go to settings)
            else if (current_mode == BREW_MODE)
                m_display.brewDisplay(currentTemp, brewTemp);
            else
                m_display.steamDisplay(currentTemp, steamTemp);
            break;
        case Display::SETTINGS_SCREEN:
            if (pressed == ButtonHandler::UP) {
                switch (m_display.option) {
                    case Display::BREW: brewTemp += 1.0; break;
                    case Display::STEAM: steamTemp += 1.0; break;
                    case Display::KP: kp += 1.0; break;
                    case Display::KI: ki += 1.0; break;
                    case Display::KD: kd += 1.0; break;
                }
                m_display.settingsDisplay(brewTemp, steamTemp, kp, ki, kd);

            }
            else if (pressed == ButtonHandler::DOWN) {
                switch (m_display.option) {
                    case Display::BREW: brewTemp -= 1.0; break;
                    case Display::STEAM: steamTemp -= 1.0; break;
                    case Display::KP: kp -= 1.0; break;
                    case Display::KI: ki -= 1.0; break;
                    case Display::KD: kd -= 1.0; break;
                }
                m_display.settingsDisplay(brewTemp, steamTemp, kp, ki, kd);
            }
            else if (pressed == ButtonHandler::SELECT) {
                switch (m_display.option) {
                    case Display::BREW: m_display.option = Display::STEAM; break;
                    case Display::STEAM: m_display.option = Display::KP; break;
                    case Display::KP: m_display.option = Display::KI; break;
                    case Display::KI: m_display.option = Display::KD; break;
                    case Display::KD: m_display.option = Display::BREW; break;
                }
                m_display.settingsDisplay(brewTemp, steamTemp, kp, ki, kd);

            }
            else if (pressed == ButtonHandler::BACK) {
                // save settings and go back to brew / steam screen
                m_eeprom.writeSettings(brewTemp, steamTemp, kp, ki, kd);
                m_pid.begin(kp, ki, kd);
                if (current_mode == BREW_MODE)
                    m_display.brewDisplay(currentTemp, brewTemp);
                else
                    m_display.steamDisplay(currentTemp, steamTemp);
            }
    }
    delay(100); // update every 100ms
}

GaggiaPID::Mode GaggiaPID::checkMode() {
    return digitalRead(m_switchPin) == HIGH ? BREW_MODE : STEAM_MODE;
}