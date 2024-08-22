// Stuff for display
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>

// Stuff for Thermocouple
#include <Adafruit_MCP9601.h>
#include <PID_v2.h>

// Stuff for communication
#include <Bounce2.h>
#include <Wire.h>

// Stuff for persistent memory to store temperature values
#include <EEPROM.h>

// Pin locations and stuff
const uint8_t upPin = 2; //red
const uint8_t downPin = 3; //blue
const uint8_t selectPin = 4; //green
const uint8_t backPin = 5; //yellow

// setup for MCP9601
#define tc_address (0x67)
Adafruit_MCP9601 mcp;

// setup for display
#define disp_address (0x3C)

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

enum ScreenType {
  HOME_SCREEN = 0,
  BREW_SCREEN,
  STEAM_SCREEN,
  PID_SCREEN
} screen;

// setup for buttons
Bounce upButton = Bounce();
Bounce downButton = Bounce();
Bounce selectButton = Bounce();
Bounce backButton = Bounce();
const unsigned long intervalMs = 5;

// setup for EEPROM values
const uint8_t FLAG_ADDRESS = 0;
const uint8_t INIT_FLAG = 0xAA;

// defaults and variables for temperature
const uint8_t BREWTEMP = 93; // default
const uint8_t STEAMTEMP = 155; // default
uint8_t brewTemp, steamTemp;

// defaults and variables for PID constants
const uint8_t KP = 30; // default
const uint8_t KI = 40; // default
const uint8_t KD = 50; // default
uint8_t kp, ki, kd;

// variables for display and control
float currentTemp; // will be fed from TC
uint8_t valueShown;
uint16_t windowStartTime;
uint16_t windowSize = 100;



void setup() {
  // put your setup code here, to run once:

  // Check if EEPROM is initialized
  if (EEPROM.read(FLAG_ADDRESS) != INIT_FLAG) {
    // EEPROM is not initialized, so write the default values
    EEPROM.write(1, BREWTEMP);
    EEPROM.write(2, STEAMTEMP);
    EEPROM.write(3, KP);
    EEPROM.write(4, KI);
    EEPROM.write(5, KD);
    EEPROM.write(FLAG_ADDRESS, INIT_FLAG);  // Set the flag to indicate initialization
  }
  brewTemp = EEPROM.read(1);
  steamTemp = EEPROM.read(2);
  kp = EEPROM.read(3);
  ki = EEPROM.read(4);
  kd = EEPROM.read(5);

  display.begin(SSD1306_SWITCHCAPVCC, disp_address);
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("hello");
  display.println("fuck you");
  display.println("coffee <3");

  // starting the TCA
  if (! mcp.begin(tc_address)) {
    display.clearDisplay();
    display.println("TC ERROR");
    display.display();
    while (1);
  }
  mcp.setADCresolution(MCP9600_ADCRESOLUTION_18);
  mcp.setThermocoupleType(MCP9600_TYPE_K);
  mcp.setFilterCoefficient(3);
  mcp.enable(true);

  display.display();
  delay(4000);

  pinMode(upPin, INPUT_PULLUP);
  pinMode(downPin, INPUT_PULLUP);
  pinMode(selectPin, INPUT_PULLUP);
  pinMode(backPin, INPUT_PULLUP);

  upButton.attach(upPin);
  upButton.interval(intervalMs);
  downButton.attach(downPin);
  downButton.interval(intervalMs);
  selectButton.attach(selectPin);
  selectButton.interval(intervalMs);
  backButton.attach(backPin);
  backButton.interval(intervalMs);

  windowStartTime = millis();

  homeDisplay();
}

void homeDisplay() {
  screen = HOME_SCREEN;
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("frick u");
  display.println("dumb fuc");
  display.println("cheeked up");
  display.display();
}

void brewDisplay() {
  screen = BREW_SCREEN;
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("Temp:     ");
  display.setCursor(60,0);
  display.println(currentTemp);

  display.println("   Brew:");
  display.print("      C");
  display.setCursor(30, display.getCursorY());
  display.println(valueShown);
  display.display();
}

void steamDisplay() {
  screen = STEAM_SCREEN;
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("Temp:    C");
  display.setCursor(70,0);
  display.println(currentTemp);

  display.println("  Steam:");
  display.print("      C");
  display.setCursor(30, display.getCursorY());
  display.println(valueShown);
  display.display();
}

void pidDisplay() {
  screen = PID_SCREEN;
  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(0,0);
  display.println("PID stuff");
  display.display();
}

void checkButtons() {

  // Before we check the other buttons, gotta check if you're exiting to home screen
  // this would trump any other buttons being pressed
  backButton.update();
  if (backButton.fell()) {
    homeDisplay();
    return;
  }

  // not going to main menu? then get latest updates on other button info!
  upButton.update();
  downButton.update();
  selectButton.update();

  // switch case to handle different logic depending on what screen we are on
  // if on home screen, then our 3 buttons go to other screens
  // if on brew screen, then the buttons will change and select the brew temp
  // if on steam screen, then the buttons will change and select the steam temp
  // if on PID screen, then the buttons will change and select PID constants
  switch (screen) {
    case HOME_SCREEN:
      if (upButton.fell()) {
        valueShown = brewTemp;
        brewDisplay();
      }
      else if (downButton.fell()) {
        valueShown = steamTemp;
        steamDisplay();
      }
      else if (selectButton.fell()) {
        pidDisplay();
      }
      break;
    
    case BREW_SCREEN:
      if (upButton.fell()) { 
        valueShown++;
      }
      else if (downButton.fell()) {
        valueShown--;
      }
      else if (selectButton.fell()) {
        // save to EEPROM
        brewTemp = valueShown;
        EEPROM.write(1, brewTemp);
        display.print("cool!");
        display.display();
        delay(500);
      }
      brewDisplay();
      break;

    case STEAM_SCREEN:
      if (upButton.fell()) {
        valueShown++;
      }
      else if (downButton.fell()) {
        valueShown--;
      }
      else if (selectButton.fell()) {
        // save to EEPROM
        steamTemp = valueShown;
        EEPROM.write(2, steamTemp);
        display.print("cool!");
        display.display();
        delay(500);
      }
      steamDisplay();
      break;

    case PID_SCREEN:
      break;
  }
}

void loop() {
  checkButtons();
  // update current temp every 100 ms
  if( millis() - windowStartTime > windowSize) {
    currentTemp = mcp.readThermocouple();
  }
}
