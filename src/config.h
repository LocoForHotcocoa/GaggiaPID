// config.h
#pragma once
#include <stdint.h>

// #include <PID_v1_bc.h>

// Pin locations and stuff
const uint8_t UP_PIN = 2; //red
const uint8_t DOWN_PIN = 3; //blue
const uint8_t SELECT_PIN = 4; //green
const uint8_t BACK_PIN = 5; //yellow
const uint8_t RELAY_PIN = 6;

// i2c addresses
const uint8_t TC_ADDRESS = 0x67;
const uint8_t DISP_ADDRESS = 0x3C;

// display resolution
const uint8_t SCREEN_WIDTH = 128;
const uint8_t SCREEN_HEIGHT = 64;
const uint8_t OLED_RESET = 4;

// --- EEPROM Addresses ---
const int FLAG_ADDR = 0x0;
const uint8_t INIT_FLAG = 170;

const int BREW_ADDR = 0x1;
const int STEAM_ADDR = 0x2;
const int KP_ADDR = 0x3;
const int KI_ADDR = 0x4;
const int KD_ADDR = 0x5;


// --- Default Values ---
const uint8_t BREWTEMP_DEFAULT = 93;   // default brew temperature
const uint8_t STEAMTEMP_DEFAULT = 155; // default steam temperature

const uint8_t KP_DEFAULT = 30; // default PID Kp
const uint8_t KI_DEFAULT = 40; // default PID Ki
const uint8_t KD_DEFAULT = 50; // default PID Kd

// --- PID Window ---
const uint8_t PID_WINDOW_SIZE = 500;  // PWM window size in ms

// --- shared external variables and objects ---
// extern Adafruit_SSD1306 display;

// extern Bounce upButton;
// extern Bounce downButton;
// extern Bounce selectButton;
// extern Bounce backButton;
const uint16_t INPUT_INTERVAL_MS = 25;

// extern Adafruit_MCP9601 mcp;

// extern double brewTemp, steamTemp, currentTemp;
// extern double kp, ki, kd;

// enum Screen { BREW_SCREEN, STEAM_SCREEN, PID_SCREEN, TEMP_SCREEN };
// extern Screen screen;
