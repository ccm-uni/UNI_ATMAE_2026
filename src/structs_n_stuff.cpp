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