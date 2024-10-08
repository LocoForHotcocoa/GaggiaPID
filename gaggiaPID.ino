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
const uint8_t relayPin = 6;

// setup for MCP9601
#define tc_address (0x67)
Adafruit_MCP9601 mcp;

// setup for display
#define disp_address (0x3C)

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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

// variables and enums for display and control
double currentTemp;  // will be fed from TC
double output;       // for PWM control
double targetTemp;  // will be fed into PID algorithm
//double valueShown;
uint32_t windowStartTime;
uint8_t windowSize = 500; // 500 ms = 2 Hz PWM

// setup for PID
PID myPID(&currentTemp, &output, &targetTemp, kp, ki, kd, DIRECT);

// used to keep track of current screen
enum ScreenType {
  BREW_SCREEN = 0,
  STEAM_SCREEN,
  PID_SCREEN
} screen, temporaryScreen;

// used to keep track of current PID option on PID screen
enum pidOptionEnum {
  KP_OPTION = 0,
  KI_OPTION,
  KD_OPTION
} pidOption;


void setup() {
  // put your setup code here, to run once:
  // Check if EEPROM is initialized
  // I use a initialization flag at 0x0 to assure that the EEPROM has saved values
  // if flag address is empty, then we need to use the defaults and save them to EEPROM (defined above)
  if (EEPROM.read(FLAG_ADDRESS) != INIT_FLAG) {
    // EEPROM is not initialized, so write the default values
    EEPROM.write(1, BREWTEMP);
    EEPROM.write(2, STEAMTEMP);
    EEPROM.write(3, KP);
    EEPROM.write(4, KI);
    EEPROM.write(5, KD);
    EEPROM.write(FLAG_ADDRESS, INIT_FLAG);  // Set the flag to indicate initialization
  }

  // Now that we are sure EEPROM has values, we read the values from EEPROM into our program
  brewTemp = EEPROM.read(1);
  steamTemp = EEPROM.read(2);
  kp = EEPROM.read(3);
  ki = EEPROM.read(4);
  kd = EEPROM.read(5);

  // initializing display, show little splash screen
  display.begin(SSD1306_SWITCHCAPVCC, disp_address);
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("coffee <3");

  // starting the TCA
  // if mcp isn't working, then the arduino must be restarted
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
  delay(2000);

  // setting up pins and buttons
  pinMode(upPin, INPUT_PULLUP);
  pinMode(downPin, INPUT_PULLUP);
  pinMode(selectPin, INPUT_PULLUP);
  pinMode(backPin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);

  upButton.attach(upPin);
  upButton.interval(intervalMs);
  downButton.attach(downPin);
  downButton.interval(intervalMs);
  selectButton.attach(selectPin);
  selectButton.interval(intervalMs);
  backButton.attach(backPin);
  backButton.interval(intervalMs);

  // setting up more PID stuff
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, windowSize);  // tell the PID to range between 0 and the full window size

  // getting current time
  windowStartTime = millis();
  brewDisplay();
  temporaryScreen = screen;
}
// void homeDisplay() {
//   screen = HOME_SCREEN;
//   display.clearDisplay();
//   display.setCursor(0,0);
//   display.setTextSize(2);
//   display.setTextColor(WHITE);
//   display.println("Hello :)");
//   display.println(currentTemp);
//   display.display();
// }

// this will be a cute little coffee mug with temperature on it!
void brewDisplay() {
  screen = BREW_SCREEN;
  targetTemp = brewTemp;
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("Temp: " + String(currentTemp));
  display.println("   Brew:");
  display.println("  " + String(brewTemp) + " C");
  display.display();
}

// this will be a cute little steam wand!
void steamDisplay() {
  screen = STEAM_SCREEN;
  targetTemp = steamTemp;
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("Temp: " + String(currentTemp));
  display.println("  Steam:");
  display.println("  " + String(steamTemp) + " C");
  display.display();
}

void pidDisplay() {
  screen = PID_SCREEN;
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("   PID");
  display.println("kp  =  " + String(kp));
  display.println("ki  =  " + String(ki));
  display.println("kd  =  " + String(kd));
  display.setCursor(60, (pidOption+1)*16);
  display.print(">");
  display.display();
}

void checkButtons() {

  // Before we check the other buttons, gotta check if you're going into / out of PID settings
  // this would trump any other buttons being pressed
  backButton.update();
  if (backButton.fell()) {
    if (screen == PID_SCREEN && temporaryScreen == BREW_SCREEN) {
      brewDisplay();
    }
    else if (screen == PID_SCREEN && temporaryScreen == STEAM_SCREEN) {
      steamDisplay();
    }
    else {
      temporaryScreen = screen;
      pidDisplay();
    }
    return;
  }

  // not going to main menu? then get latest updates on other button info!
  upButton.update();
  downButton.update();
  selectButton.update();

  // switch case to handle different logic depending on what screen we are on
  // if on brew screen, then the buttons will change and select the brew temp or go to steam screen
  // if on steam screen, then the buttons will change and select the steam temp or go to brew screen
  // if on PID screen, then the buttons will change and select PID constants
  switch (screen) {

    case BREW_SCREEN:
      if (upButton.fell()) { 
        brewTemp++;
        EEPROM.write(1, brewTemp); // save to EEPROM
        brewDisplay();
      }
      else if (downButton.fell()) {
        brewTemp--;
        EEPROM.write(1, brewTemp); // save to EEPROM
        brewDisplay();

      }
      else if (selectButton.fell()) {
        steamDisplay();
      }
      break;

    case STEAM_SCREEN:
      if (upButton.fell()) {
        steamTemp++;
        EEPROM.write(2, steamTemp); // save to EEPROM
        steamDisplay();
      }
      else if (downButton.fell()) {
        steamTemp--;
        EEPROM.write(2, steamTemp); // save to EEPROM
        steamDisplay();
      }
      else if (selectButton.fell()) {
        brewDisplay();
      }
      break;

    case PID_SCREEN:
      if (upButton.fell()) {
        switch (pidOption) {
          case KP_OPTION: 
            kp++;
            EEPROM.write(3, kp);
            break;
          case KI_OPTION: 
            ki++;
            EEPROM.write(4, ki);
            break;
          case KD_OPTION:
            kd++;
            EEPROM.write(5, kd);
        }
        myPID.SetTunings(kp, ki, kd);
      }
      else if (downButton.fell()) {
        switch (pidOption) {
          case KP_OPTION: 
            kp--; 
            EEPROM.write(3, kp);
            break;
          case KI_OPTION: 
            ki--;
            EEPROM.write(4, ki);
            break;
          case KD_OPTION: 
            kd--;
            EEPROM.write(5, kd);
        }
        myPID.SetTunings(kp, ki, kd);
      }
      // when we press select, it goes to next PID variable
      else if (selectButton.fell()) {
        switch (pidOption) {
            case KP_OPTION: pidOption = KI_OPTION; break; // Switch to Ki setting
            case KI_OPTION: pidOption = KD_OPTION; break; // Switch to Kd setting
            case KD_OPTION: pidOption = KP_OPTION;        // Switch to Kp setting
        }
      }
      pidDisplay();
  }
}

void loop() {
  checkButtons();

  // update everything every 500 ms
  // this is some goofy logic that I got from the PID_v2 arduino docs that's supposed to work with a digital output (instead of PWM). I hope it works!
  while ( millis() - windowStartTime > windowSize) {
    currentTemp = mcp.readThermocouple();
    myPID.Compute();
    windowStartTime += windowSize;

    if (output < millis() - windowStartTime)
      digitalWrite(relayPin, HIGH);
    else
      digitalWrite(relayPin, LOW);
  }
}
