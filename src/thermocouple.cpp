// controls adafruit MCP9601 TCA

#include "thermocouple.h"

Thermocouple::Thermocouple() = default;

bool Thermocouple::begin(TwoWire &wire, 
                         uint8_t addr, 
                         _resolution res, 
                         _themotype type, 
                         uint8_t filterCoef) {
    wire.begin();
    if (! m_mcp.begin(addr, &wire)) {
        return false;
    }

    m_mcp.setADCresolution( res );
    m_mcp.setThermocoupleType( type );
    m_mcp.setFilterCoefficient( filterCoef );
    m_mcp.enable(true);

    return true;
}

float Thermocouple::readCelsius() {
    return m_mcp.readThermocouple();
}