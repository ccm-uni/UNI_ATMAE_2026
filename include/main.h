#ifndef main_h
#define main_h

#include <Arduino.h>
#include <Basicmicro.h>
#include <Bluepad32.h>

#include "structs_n_stuff.cpp"


// Need this for the controllers to work
ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// Creates motordriver class connected to Serial2, with 5000us timout
Basicmicro motorDriver(&Serial2, 5000);


// All controller data
ControllerStruct controller;


void onConnectedController(ControllerPtr ctl);
void onDisconnectedController(ControllerPtr ctl);
void processControllers();
void processGamepad(ControllerPtr ctl);

#endif
