// PIDController.h

#pragma once

#include <Arduino.h>

class PIDController {
public:
    PIDController();
    bool begin(const float& kp, const float& ki, const float& kd);
    float update(const float& setpoint, const float& measured);
    float kp, ki, kd;
private:
    float m_integral, m_lastError;
    static constexpr float m_dt = 0.1;
};