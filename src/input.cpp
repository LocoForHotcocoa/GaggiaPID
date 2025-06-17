#include "input.h"

Input::Input(uint8_t upPin, uint8_t downPin, 
             uint8_t selectPin, uint8_t backPin, 
             uint16_t interval) 
{
    up.attach(upPin);
    down.attach(downPin);
    select.attach(selectPin);
    back.attach(backPin);
    
    up.interval(interval);
    down.interval(interval);
    select.interval(interval);
    back.interval(interval);
}

void Input::update() 
{
    back.update();
    up.update();
    down.update();
    select.update();
}