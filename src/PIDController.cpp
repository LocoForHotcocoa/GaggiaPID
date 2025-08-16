#include "PIDController.h"

PIDController::PIDController() = default;

bool PIDController::begin(float& kp, float& ki, float& kd) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
    return true;
}

float PIDController::update(float& setpoint, float& measured) {
    float error = setpoint - measured;

    // Proportional term
    float p_out = kp * error;

    // Integral term with anti-windup
    // We will calculate the integral contribution separately
    float integral_contribution = ki * (m_integral + error * m_dt);

    // Derivative term
    float derivative = (error - m_lastError) / m_dt;
    float d_out = kd * derivative;
    m_lastError = error;

    // Total PID output
    float output = p_out + integral_contribution + d_out;

    // Clamp the output to the valid range (0.0 to 1.0 for SSR duty cycle)
    float clamped_output = constrain(output, 0.0f, 1.0f);

    // Anti-windup logic:
    // Only update the integral if the output is not saturated,
    // or if the integral is trying to pull the output back from saturation.
    if (clamped_output == output) {
        m_integral += error * m_dt;
    }

    return clamped_output;
}