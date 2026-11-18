#pragma once

#ifndef main_h
#define main_h

#include <Arduino.h>
#include <Basicmicro.h>
#include <Bluepad32.h>
#include <ESP32Servo.h>

#include "bt_controller.h"
#include "structs_n_stuff.h"
#include "color.h"


#define driver1Addr 128 // RoboClaw 1 Address
#define driver2Addr 129 // RoboClaw 2 Address
#define driver3Addr 130 // RoboClaw 3 Address


ESP_IO gpio;  // GPIO map struct
Robot  robot; // All physical stuff on the robot

// Creates motordriver class connected to Serial2, with 5000us timout
Basicmicro motorDriver(&Serial2, 5000);


void initGPIO();
void setColorBar();
void adjustDriveSpeed();
void calcMech();
void driveMotors(int powFL, int powFR, int powBL, int powBR);
void liftCtl(int pow);

void autoMode();
void setServos();

void getCycleTime();
void printDebugController();

#endif
