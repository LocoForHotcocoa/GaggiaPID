// input.h
#pragma once

#include <Bounce2.h>
#include "config.h"

class Input {
public:
    Input();
    void checkButtons();
private:
    Bounce up, down, select, back;
};
