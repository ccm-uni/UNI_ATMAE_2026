#pragma once
#include <Bluepad32.h>

struct ControllerStruct {
  int LX = 0;
  int LY = 0;
  int RX = 0;
  int RY = 0;
  int L2 = 0;
  int R2 = 0;

  bool dpad_up = false;
  bool dpad_down = false;
  bool dpad_left = false;
  bool dpad_right = false;

  bool cross = false;
  bool circle = false;
  bool triangle = false;
  bool square = false;

  bool L1 = false;
  bool R1 = false;

  bool connected = false;

  byte colorBar[3] = {0, 0, 0};
  byte deadzone = 16;
};


// Properly defined in bt_controller.cpp
// Stores the current state of the controller
extern ControllerStruct controllerData;

void onConnectedController(ControllerPtr ctl);
void onDisconnectedController(ControllerPtr ctl);
void processControllers();