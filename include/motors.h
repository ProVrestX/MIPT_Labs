#ifndef INC_MOTORS_H_
#define INC_MOTORS_H_

#include <Arduino.h>

#define MOTOR_MAX_SPEED 750
#define MOTOR_DEF_SPEED 200

extern int16_t motor_step[2];
extern uint16_t motor_speed[2];

void motor_init(void);
int8_t check_buttons(uint8_t num);
int16_t motor_set_step(uint8_t num, int16_t step);
void motor_set_phase(uint8_t num, uint8_t step);
void motor_off(uint8_t num);
void motor_zero(uint8_t num);
void motor_set_speed(uint8_t num, uint16_t speed);

#endif