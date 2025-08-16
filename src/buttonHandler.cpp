#include "buttonHandler.h"

ButtonHandler::ButtonHandler(uint8_t upPin, uint8_t downPin, 
                             uint8_t selectPin, uint8_t backPin,
                             uint16_t bounce_ms) {

    m_buttons[UP].attach(upPin, INPUT_PULLUP);
    m_buttons[DOWN].attach(downPin, INPUT_PULLUP);
    m_buttons[SELECT].attach(selectPin, INPUT_PULLUP);
    m_buttons[BACK].attach(backPin, INPUT_PULLUP);

    for(auto& btn : m_buttons) {
        btn.interval(bounce_ms);
    }
}

void ButtonHandler::update() {
    for(auto& btn : m_buttons) {
        btn.update();
    }
}

ButtonHandler::Button ButtonHandler::getPressed() {
    for(uint8_t i = 0; i < ButtonHandler::BUTTON_COUNT; i++) {
        if (m_buttons[i].fell())
            return (ButtonHandler::Button)i;
    }
    return ButtonHandler::NONE;
}