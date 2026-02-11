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
    static constexpr uint16_t DEBOUNCE_MS = 50;
    static constexpr uint8_t UP_PIN = 6; //red
    static constexpr uint8_t DOWN_PIN = 7; //blue
    static constexpr uint8_t SELECT_PIN = 8; //green
    static constexpr uint8_t BACK_PIN = 9; //yellow
    // static constexpr uint32_t LONGPRESS_MS  = 1000;
};