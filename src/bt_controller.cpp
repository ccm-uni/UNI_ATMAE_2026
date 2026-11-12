#include "bt_controller.h"
GamepadStruct gamepad;

namespace {
ControllerPtr myControllers[BP32_MAX_GAMEPADS] = {};


void processGamepad(ControllerPtr ctl) {
  ctl->setColorLED(gamepad.colorBar[0], gamepad.colorBar[1],
                   gamepad.colorBar[2]);

  gamepad.LX = ctl->axisX();
  gamepad.LY = -ctl->axisY();
  gamepad.RX = ctl->axisRX();
  gamepad.RY = -ctl->axisRY();

  gamepad.L1 = ctl->l1();
  gamepad.R1 = ctl->r1();

  gamepad.L2 = ctl->l2();
  gamepad.R2 = ctl->r2();

  gamepad.dpad_up    = ctl->dpad() & 0x01;
  gamepad.dpad_down  = ctl->dpad() & 0x02;
  gamepad.dpad_right = ctl->dpad() & 0x04;
  gamepad.dpad_left  = ctl->dpad() & 0x08;

  gamepad.cross    = ctl->a();
  gamepad.circle   = ctl->b();
  gamepad.triangle = ctl->y();
  gamepad.square   = ctl->x();

  gamepad.share   = ctl->miscSelect();
  gamepad.options = ctl->miscStart();
}
} // namespace

void onConnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);

      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n",
                    ctl->getModelName().c_str(), properties.vendor_id,
                    properties.product_id);

      myControllers[i] = ctl;
      return;
    }
  }

  Serial.println(
      "CALLBACK: Controller connected, but could not find empty slot");
}

void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      return;
    }
  }

  Serial.println(
      "CALLBACK: Controller disconnected, but not found in myControllers");
}


/**
 * Go through connected controllers, and filter to make sure the data is good
 */
void processControllers() {
  gamepad.connected = false;

  for (auto myController : myControllers) {
    if (myController == nullptr) {
      continue;
    }

    if (!myController->isConnected()) {
      Serial.println("ERROR: Controller NOT Connected");
    } else if (!myController->isGamepad() || !myController->hasData()) {
      continue;
    } else {
      processGamepad(myController);
      gamepad.connected = true;
    }
  }
}