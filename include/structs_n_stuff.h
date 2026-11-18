/*
  This is where I'm putting global variables and structs 
*/

#include <Arduino.h>

// I/O mapping of the ESP pins to the output pins
struct ESP_IO {

  // clang-format off
  const static byte
    out1 = 13,
    out2 = 12,
    out3 = 27,
    out4 = 14,
    out5 = 26,
    out6 = 25,
    out7 = 32,
    out8 = 33,
    out9 = 15,
    out10 = 2,
    out11 = 5,
    out12 = 4;

};

struct Robot {
  struct Motor {
    int16_t 
      FL_Motor = 0,
      FR_Motor = 0,
      BL_Motor = 0,
      BR_Motor = 0;
  };
  Motor motor; // Drive motors

  struct Servos
  {
    Servo wrist;
    Servo claw;
    int wristPos = 0;
    int clawPos = 0;

    bool wristUp = true;
  };
  Servos servo; // All the servos

  struct ColorSensors{
    int L_Red = 0;
    int L_Green = 0;
    int L_Blue = 0;
    int L_Wht = 0;

    int R_Red = 0;
    int R_Green = 0;
    int R_Blue = 0;
    int R_Wht = 0;
  };
  ColorSensors colors;
  

  bool slowSpeed = false;
  bool fastSpeed = false;

  bool goodToGo = false;
};

// clang-format on