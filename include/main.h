#include <Arduino.h>

#include <Basicmicro.h>
#include <Bluepad32.h>


// Need this for the controllers to work
ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void onConnectedController(ControllerPtr ctl);
void onDisconnectedController(ControllerPtr ctl);



Basicmicro motorDriver(&Serial2, 5000);