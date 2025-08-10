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

    ButtonHandler(uint8_t upPin, uint8_t downPin, 
                  uint8_t selectPin, uint8_t backPin,
                  uint16_t debounce_ms = DEBOUNCE_MS);
    void update();
    Button getPressed();
    // bool longPress(Button b);
private:
    
    Bounce m_buttons[BUTTON_COUNT];
    static constexpr uint32_t DEBOUNCE_MS = 50;
    // static constexpr uint32_t LONGPRESS_MS  = 1000;
};