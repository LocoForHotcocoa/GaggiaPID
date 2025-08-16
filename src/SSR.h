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
  unsigned long m_lastToggle;
  static constexpr uint8_t SSR_PIN = 6;
};