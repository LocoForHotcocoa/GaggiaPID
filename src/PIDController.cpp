#include "PIDController.h"

PIDController::PIDController(float kp, float ki, float kd, float dtSeconds) 
    : m_kp(kp), m_ki(ki), m_kd(kd), m_dt(dtSeconds) {}

float PIDController::update(float setpoint, float measured) {
    // chatgpt algo
    float error = setpoint - measured;
    m_integral += error * m_dt;
    float derivative = (error - m_lastError) / m_dt;
    m_lastError = error;
    return m_kp*error + m_ki*m_integral + m_kd*derivative;
}