// PIDController.h

#pragma once

class PIDController {
public:
    PIDController();
    bool begin(float& kp, float& ki, float& kd);
    float update(float& setpoint, float& measured);
    float kp, ki, kd;
private:
    float m_integral, m_lastError;
    static constexpr float m_dt = 0.1;
};