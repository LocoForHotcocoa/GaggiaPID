// config.h
#pragma once
#include <stdint.h>

// #include <PID_v1_bc.h>

// Pin locations and stuff
constexpr uint8_t upPin = 2; //red
constexpr uint8_t downPin = 3; //blue
constexpr uint8_t selectPin = 4; //green
constexpr uint8_t backPin = 5; //yellow
constexpr uint8_t relayPin = 6;

// i2c addresses
constexpr uint8_t tc_address = 0x67;
constexpr uint8_t disp_address = 0x3C;

// display resolution
constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;
constexpr uint8_t OLED_RESET = 4;

// --- EEPROM Addresses ---
constexpr int FLAG_ADDR = 0x0;
constexpr uint8_t INIT_FLAG = 170;

constexpr int BREW_ADDR = 0x1;
constexpr int STEAM_ADDR = 0x2;
constexpr int KP_ADDR = 0x3;
constexpr int KI_ADDR = 0x4;
constexpr int KD_ADDR = 0x5;


// --- Default Values ---
constexpr uint8_t BREWTEMP_DEFAULT = 93;   // default brew temperature
constexpr uint8_t STEAMTEMP_DEFAULT = 155; // default steam temperature

constexpr uint8_t KP_DEFAULT = 30; // default PID Kp
constexpr uint8_t KI_DEFAULT = 40; // default PID Ki
constexpr uint8_t KD_DEFAULT = 50; // default PID Kd

// --- PID Window ---
constexpr uint8_t windowSize = 500;  // PWM window size in ms

// --- shared external variables and objects ---
// extern Adafruit_SSD1306 display;

// extern Bounce upButton;
// extern Bounce downButton;
// extern Bounce selectButton;
// extern Bounce backButton;
constexpr int intervalMs = 25;

// extern Adafruit_MCP9601 mcp;

// extern double brewTemp, steamTemp, currentTemp;
// extern double kp, ki, kd;

// enum Screen { BREW_SCREEN, STEAM_SCREEN, PID_SCREEN, TEMP_SCREEN };
// extern Screen screen;
