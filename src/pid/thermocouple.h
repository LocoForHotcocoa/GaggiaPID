// thermocouple.h
#pragma once

#include <Adafruit_MCP9601.h>
#include <Wire.h>

class Thermocouple {
public:
    Thermocouple();
    bool begin(TwoWire &wire = Wire, 
               uint8_t addr = TC_ADDRESS, 
               _resolution res = ADC_RES, 
               _themotype type = TC_TYPE,
               uint8_t filterCoef = FILTER_COEF);

    float readCelsius();

    static constexpr uint8_t TC_ADDRESS   = 0x67;
    static constexpr _resolution ADC_RES  = MCP9600_ADCRESOLUTION_18;
    static constexpr _themotype TC_TYPE   = MCP9600_TYPE_K;
    static constexpr uint8_t FILTER_COEF  = 3;

private:
    Adafruit_MCP9601 m_mcp;
};