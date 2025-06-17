// input.h
#pragma once

#include <Bounce2.h>
#include <stdint.h>

class Input 
{
public:
    Input(uint8_t upPin, uint8_t downPin, 
        uint8_t selectPin, uint8_t backPin, 
        uint16_t interval);

    void update();
    Bounce up, down, select, back;
};
