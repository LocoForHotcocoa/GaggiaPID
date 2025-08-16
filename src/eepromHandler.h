#pragma once

#include <EEPROM.h>
#include <stdint.h>

class EEPROMHandler {
public:
    struct Settings {
        float brewTemp = BREWTEMP_DEFAULT;
        float steamTemp = STEAMTEMP_DEFAULT;
        float kp = KP_DEFAULT;
        float ki = KI_DEFAULT;
        float kd = KD_DEFAULT;
        uint32_t magic = SETTINGS_MAGIC;
        uint16_t checksum = 0;
    };
    Settings settings;
    EEPROMHandler();
    bool readSettings();
    bool writeSettings(float& brewTemp, float& steamTemp,
                       float& kp, float& ki, float& kd);

private:
    uint16_t calculateChecksum(const EEPROMHandler::Settings& s);

    Settings m_defaultSettings;
    static constexpr int SETTINGS_ADDR = 0x1;
    static constexpr float BREWTEMP_DEFAULT = 93.0;   // default brew temperature
    static constexpr float STEAMTEMP_DEFAULT = 155.0; // default steam temperature
    static constexpr float KP_DEFAULT = 30.0; // default PID Kp
    static constexpr float KI_DEFAULT = 40.0; // default PID Ki
    static constexpr float KD_DEFAULT = 50.0; // default PID Kd
    static constexpr uint32_t SETTINGS_MAGIC = 0xDEADBEEF; // magic number for EEPROM validation
};