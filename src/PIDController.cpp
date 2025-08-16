#include "PIDController.h"

PIDController::PIDController() = default;

bool PIDController::begin(float& kp, float& ki, float& kd) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
    return true;
}

float PIDController::update(float& setpoint, float& measured) {
    // chatgpt algo
    float error = setpoint - measured;
    m_integral += error * m_dt;
    float derivative = (error - m_lastError) / m_dt;
    m_lastError = error;
    return kp*error + ki*m_integral + kd*derivative;
}