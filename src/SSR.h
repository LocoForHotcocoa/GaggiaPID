#pragma once

#include <Arduino.h>

class SSR {
public:
  SSR(uint8_t pin = SSR_PIN);
  void setDuty(float duty);    // duty 0-1
  void update();               // call each loop to drive the SSR
private:
  uint8_t m_pin;
  float m_duty;
  unsigned long m_windowStartTime;
  unsigned long m_onTime;
  static constexpr unsigned long m_windowSize = 5000; // 5 second window
  static constexpr uint8_t SSR_PIN = 6;
};