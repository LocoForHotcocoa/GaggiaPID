// PIDController.h

#pragma once

class PIDController {
public:
    PIDController(float kp, float ki, float kd, float dtSeconds);
    float update(float setpoint, float measured);
private:
    float m_kp, m_ki, m_kd, m_dt;
    float m_integral, m_lastError;
};