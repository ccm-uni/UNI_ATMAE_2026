#pragma once

#ifndef main_h
#define main_h

#include <Arduino.h>
#include <Basicmicro.h>
#include <Bluepad32.h>

#include "bt_controller.h"

#include "structs_n_stuff.cpp"

// GPIO map struct
ESP_IO gpio;

// Creates motordriver class connected to Serial2, with 5000us timout
Basicmicro motorDriver(&Serial2, 5000);

#endif
