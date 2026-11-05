#include "bt_controller.h"
ControllerStruct controllerData;

namespace {
ControllerPtr myControllers[BP32_MAX_GAMEPADS] = {};


void processGamepad(ControllerPtr ctl) {
  ctl->setColorLED(controllerData.colorBar[0], controllerData.colorBar[1],
                   controllerData.colorBar[2]);

  controllerData.LX = ctl->axisX();
  controllerData.LY = -ctl->axisY();
  controllerData.RX = ctl->axisRX();
  controllerData.RY = -ctl->axisRY();

  controllerData.L2 = ctl->l2();
  controllerData.R2 = ctl->r2();

  controllerData.dpad_up = ctl->dpad() & 0x01;
  controllerData.dpad_down = ctl->dpad() & 0x02;
  controllerData.dpad_right = ctl->dpad() & 0x04;
  controllerData.dpad_left = ctl->dpad() & 0x08;

  controllerData.cross = ctl->a();
  controllerData.circle = ctl->b();
  controllerData.triangle = ctl->y();
  controllerData.square = ctl->x();

  controllerData.L1 = ctl->l1();
  controllerData.R1 = ctl->r1();
}
}  // namespace

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

void processControllers() {
  controllerData.connected = false;

  for (auto myController : myControllers) {
    if (myController == nullptr) {
      continue;
    }

    if (!myController->isConnected()) {
      Serial.println("ERROR: Controller NOT Connected");
    } else if (!myController->hasData()) {
      Serial.println("ERROR: Controller has no Data");
    } else if (!myController->isGamepad()) {
      Serial.println("ERROR: Controller is NOT a Gamepad");
    } else {
      processGamepad(myController);
      controllerData.connected = true;
    }
  }
}