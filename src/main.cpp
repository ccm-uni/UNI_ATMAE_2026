#include "main.h"

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n",
                    ctl->getModelName().c_str(), properties.vendor_id,
                    properties.product_id);
      myControllers[i] = ctl;
      foundEmptySlot   = true;
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println(
        "CALLBACK: Controller connected, but could not found empty slot");
  }
}


// When a controller disconnects
void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      foundController  = true;
      break;
    }
  }

  if (!foundController) {
    Serial.println(
        "CALLBACK: Controller disconnected, but not found in myControllers");
  }
}


// Determine which controller proccessor to use and verifies the controller data
void processControllers() {
  // While trying to process the controller, if it doesn't get the proper data,
  // makes this false
  controller.connected = false;

  for (auto myController : myControllers) {
    // Controller variable error
    if (!myController) {
    } else if (!myController->isConnected()) { // No Controller connected
      Serial.println("ERROR: Controller NOT Connected");

    } else if (!myController->hasData()) { // No data
      Serial.println("ERROR: Controller has no Data");

    } else if (!myController->isGamepad()) { // Not a supported gamepad
      Serial.println("ERROR: Controller is NOT a Gamepad");

    } else { // Regular run
      processGamepad(myController);
      // Confirms controller is connected
      controller.connected = true;
    }
  }
}


// Map all BT32 controller values to local struct
void processGamepad(ControllerPtr ctl) {
  // Update color bar
  ctl->setColorLED(controller.colorBar[0], controller.colorBar[1],
                   controller.colorBar[2]);

  controller.LX = ctl->axisX();
  controller.LY = -ctl->axisY();
  controller.RX = ctl->axisRX();
  controller.RY = -ctl->axisRY();

  controller.L2 = ctl->l2();
  controller.R2 = ctl->r2();

  controller.dpad_up    = ctl->dpad() & 0x01;
  controller.dpad_down  = ctl->dpad() & 0x02;
  controller.dpad_right = ctl->dpad() & 0x04;
  controller.dpad_left  = ctl->dpad() & 0x08;

  controller.cross    = ctl->a();
  controller.circle   = ctl->b();
  controller.triangle = ctl->y();
  controller.square   = ctl->x();

  controller.L1 = ctl->l1();
  controller.R1 = ctl->r1();


  // Another way to query controller data is by getting the buttons() function.
  // See how the different "dump*" functions dump the Controller info.
  // dumpGamepad(ctl);
}


void setup() {
  Serial.begin(115200);
  motorDriver.begin(38400); // Initialize the motor drivers

  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t *addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2],
                addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedController, &onDisconnectedController);


  // "forgetBluetoothKeys()" should be called when the user performs
  // a "device factory reset", or similar.
  // Calling "forgetBluetoothKeys" in setup() just as an example.
  // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
  // But it might also fix some connection / re-connection issues.
  // BP32.forgetBluetoothKeys();

  // Enables mouse / touchpad support for gamepads that support them.
  // When enabled, controllers like DualSense and DualShock4 generate two
  // connected devices:
  // - First one: the gamepad
  // - Second one, which is a "virtual device", is a mouse.
  // By default, it is disabled.
  BP32.enableVirtualDevice(false);
}


void loop() {
  // Updates the controller once a timer is done.
  if (BP32.update()) {
    processControllers();
  }

}