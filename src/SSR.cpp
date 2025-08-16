#include "SSR.h"

SSR::SSR(uint8_t pin) : m_pin(pin), m_duty(0), m_lastToggle(0) {
    pinMode(pin, OUTPUT);
}

void SSR::setDuty(float duty) {
    m_duty = constrain(duty, 0.0f, 1.0f);
}

void SSR::update() {
    // NOTE: currently straight from chatgpt. i have no idea what this does
    // simple bang-bang PWM or time-slice
    unsigned long now = millis();
    unsigned long period = 1000; // 1s slicing
    unsigned long onTime = (unsigned long)(period * m_duty);
    if ((now % period) < onTime) digitalWrite(m_pin, HIGH);
    else digitalWrite(m_pin, LOW);
}