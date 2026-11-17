#ifndef bt_controller_h
#define bt_controller_h
#include <Bluepad32.h>

// clang-format off
struct GamepadStruct {
  int
    LX = 0,
    LY = 0,
    RX = 0,
    RY = 0,
    L2 = 0,
    R2 = 0;

  bool
    dpad_up = false,
    dpad_down = false,
    dpad_left = false,
    dpad_right = false,

    cross = false,
    circle = false,
    triangle = false,
    square = false,

    L1 = false,
    R1 = false,

    share   = false,
    options = false;


  bool connected = false;

  // RGB color bar
  byte colorBar[3] = {0, 0, 0};

  // Deadzone used for joysticks
  byte deadzone = 16;
};
// clang-format on

// Properly defined in bt_controller.cpp
// Stores the current state of the controller
extern GamepadStruct gamepad;

void onConnectedController(ControllerPtr ctl);
void onDisconnectedController(ControllerPtr ctl);
void processControllers();

#endif