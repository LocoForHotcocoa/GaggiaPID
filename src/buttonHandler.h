// buttonHandler.h
// I commented out all the 'longpress' button options because there isn't a use in my ui rn.

#pragma once

#include <stdint.h>
#include <Bounce2.h>

class ButtonHandler {
public:

    enum Button {
        UP = 0,
        DOWN,
        SELECT,
        BACK,
        BUTTON_COUNT,
        NONE
    };

    ButtonHandler(uint8_t upPin = UP_PIN, uint8_t downPin = DOWN_PIN, 
                  uint8_t selectPin = SELECT_PIN, uint8_t backPin = BACK_PIN,
                  uint16_t debounce_ms = DEBOUNCE_MS);
    void update();
    Button getPressed();
    // bool longPress(Button b);
private:
    
    Bounce m_buttons[BUTTON_COUNT];
    static constexpr uint32_t DEBOUNCE_MS = 50;
    static constexpr uint8_t UP_PIN = 2; //red
    static constexpr uint8_t DOWN_PIN = 3; //blue
    static constexpr uint8_t SELECT_PIN = 4; //green
    static constexpr uint8_t BACK_PIN = 5; //yellow
    static constexpr uint8_t RELAY_PIN = 6;
    // static constexpr uint32_t LONGPRESS_MS  = 1000;
};