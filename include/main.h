#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include <Arduino.h>

const uint8_t BUTTON_PINS[2][2] = {
  {A1, A0},
  {A2, A3}
};

const uint8_t MOTOR_PINS[2][4] = {
  {2, 3, 4, 5},
  {9, 8, 7, 6}
};

const uint8_t PHASE[4][4] = {
  {1, 0, 0, 1},
  {1, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 1}
};


#endif