#pragma once

#ifndef main_h
#define main_h

#include <Arduino.h>
#include <Basicmicro.h>
#include <Bluepad32.h>
#include <ESP32Servo.h>

#include "bt_controller.h"
#include "structs_n_stuff.cpp"


#define driver1Addr 0x80  // RoboClaw 1 Address
#define driver2Addr 0x81  // RoboClaw 2 Address


ESP_IO gpio;// GPIO map struct
Robot robot; // All physical stuff on the robot

// Creates motordriver class connected to Serial2, with 5000us timout
Basicmicro motorDriver(&Serial2, 5000);


Servo wrist, claw;


void initGPIO();
void calcMech();
void adjustDriveSpeed();
void driveMotors(int powFL, int powFR, int powBL, int powBR);

void getCycleTime();
void printDebugController();

#endif
