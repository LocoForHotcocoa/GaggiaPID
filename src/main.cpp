// src/main.cpp
// Main entry point for the GaggiaPID project
// Author: Matthew Bradley

#include <Arduino.h>
#include "gaggiaPID.h"

GaggiaPID gaggia;

void setup() {
    gaggia.begin();
}

void loop() {
    gaggia.menuLoop();
}