#include "SSR.h"

SSR::SSR(uint8_t pin) : m_pin(pin), m_duty(0), m_windowStartTime(0), m_onTime(0) {
    pinMode(m_pin, OUTPUT);
    digitalWrite(m_pin, LOW); // Ensure SSR is off initially
}

void SSR::setDuty(float duty) {
    m_duty = constrain(duty, 0.0f, 1.0f);
}

void SSR::update() {
    unsigned long now = millis();

    // Check if we are at the start of a new time window
    if (now - m_windowStartTime > m_windowSize) {
        m_windowStartTime = now;
        // Calculate the time the SSR should be on for this window
        m_onTime = (unsigned long)(m_windowSize * m_duty);
    }


    // Turn the SSR on or off based on the time within the window
    if (now - m_windowStartTime < m_onTime) {
        digitalWrite(m_pin, HIGH);
    } else {
        digitalWrite(m_pin, LOW);
    }
}