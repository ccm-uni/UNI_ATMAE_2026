#include <Arduino.h>

#include <Basicmicro.h>
#include <Bluepad32.h>


// Need this for the controllers to work
ControllerPtr myControllers[BP32_MAX_GAMEPADS];
Basicmicro motorDriver(&Serial2, 5000);

// Easy way to reach the controller values
struct ControllerStruct {
  int16_t
    LX = 0,
    LY = 0,
    RX = 0,
    RY = 0,
    L2 = 0,
    R2 = 0;

  bool
    dpad_up,
    dpad_down,
    dpad_left,
    dpad_right;

  bool
    cross = false,
    circle = false,
    triangle = false,
    square = false;

  bool
    L1 = false,
    R1 = false;

  bool
    connected = false;

  byte
    colorBar[3] = {0, 0, 0};

  byte deadzone = 16;
};
ControllerStruct controller;


void onConnectedController(ControllerPtr ctl);
void onDisconnectedController(ControllerPtr ctl);
void processControllers();
void processGamepad(ControllerPtr ctl);