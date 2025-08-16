#include "eepromHandler.h"

EEPROMHandler::EEPROMHandler() = default;

bool EEPROMHandler::readSettings() {
    Settings readSettings;
    EEPROM.get(SETTINGS_ADDR, readSettings);
    // Validate magic number
    if (readSettings.magic != SETTINGS_MAGIC || 
        readSettings.checksum != calculateChecksum(readSettings)) {
        // Return defaults if magic number doesn't match
        settings = m_defaultSettings;
        settings.checksum = calculateChecksum(settings);
        EEPROM.put(SETTINGS_ADDR, settings);
        return false;
    }
    else {
        settings = readSettings;
        return true;
    }
}

bool EEPROMHandler::writeSettings(float& brewTemp, float& steamTemp,
                               float& kp, float& ki, float& kd) {
    settings.brewTemp = brewTemp;
    settings.steamTemp = steamTemp;
    settings.kp = kp;
    settings.ki = ki;
    settings.kd = kd;
    settings.checksum = calculateChecksum(settings);
    EEPROM.put(SETTINGS_ADDR, settings);
    return true;
}
uint16_t EEPROMHandler::calculateChecksum(const EEPROMHandler::Settings& s) {
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&s);
    size_t len = sizeof(EEPROMHandler::Settings) - sizeof(s.checksum);
    uint16_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        sum += ptr[i];
    }
    return sum;
}