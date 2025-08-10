#include "buttonHandler.h"

ButtonHandler::ButtonHandler(uint8_t upPin, uint8_t downPin, 
                             uint8_t selectPin, uint8_t backPin) {
    m_buttons[UP].pin = upPin;
    m_buttons[DOWN].pin = downPin;
    m_buttons[SELECT].pin = selectPin;
    m_buttons[BACK].pin = backPin;
}

void ButtonHandler::begin(uint16_t bounce_ms) {
// sets up all pins as `INPUT_PULLUP`
    for(auto& btn : m_buttons) {
        Bounce& bounce = btn.bounce;
        uint8_t pin = btn.pin;

        bounce.attach(pin, INPUT_PULLUP);
        bounce.interval(bounce_ms);
    }
}

void ButtonHandler::update() {
    for(auto& btn : m_buttons) {
        btn.bounce.update();
    }
}

ButtonHandler::Button ButtonHandler::getPressed() {
    for(uint8_t i = 0; i < ButtonHandler::BUTTON_COUNT; i++) {
        if (m_buttons[i].bounce.fell())
            return (ButtonHandler::Button)i;
    }
    return ButtonHandler::NONE;
}