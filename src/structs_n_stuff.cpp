/*
  This is where I'm putting global variables and structs 
*/

#include <Arduino.h>

// I/O mapping of the ESP pins to the output pins
struct ESP_IO {
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
  Motor motor;

  int16_t test = 0;


  bool mediumSpeed = false;
  bool fastSpeed = false;
};