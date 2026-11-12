#include "main.h"

void initGPIO() {
  pinMode(gpio.out1, OUTPUT);
  pinMode(gpio.out2, OUTPUT);
  pinMode(gpio.out3, OUTPUT);
  pinMode(gpio.out4, OUTPUT);
  pinMode(gpio.out5, OUTPUT);
  pinMode(gpio.out6, OUTPUT);
  pinMode(gpio.out7, OUTPUT);
  pinMode(gpio.out8, OUTPUT);
  pinMode(gpio.out9, OUTPUT);
  pinMode(gpio.out10, OUTPUT);
  pinMode(gpio.out11, OUTPUT);
  pinMode(gpio.out12, OUTPUT);
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

  BP32.enableVirtualDevice(true);
  // BP32.forgetBluetoothKeys(); // Call here if you want to clear paired
  // devices.

  initGPIO();
}


void loop() {
  // Updates the controller data
  if (BP32.update()) {
    processControllers();
  }

  if (controllerData.cross) {
    controllerData.colorBar[0] = 255;
    controllerData.colorBar[1] = 0;
    controllerData.colorBar[2] = 0;
  } else {
    controllerData.colorBar[0] = 0;
    controllerData.colorBar[1] = 255;
    controllerData.colorBar[2] = 0;
  }
}