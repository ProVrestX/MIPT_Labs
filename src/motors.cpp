#include "main.h"
#include "motors.h"

int16_t motor_step[2] = {0};
uint16_t motor_speed[2] = {MOTOR_DEF_SPEED, MOTOR_DEF_SPEED};

void motor_init(void) {
    uint16_t counter = 0;

    for(uint8_t i = 0; i < 2; ++i) {
        counter = 0;

        while(check_buttons(i) != -1) {
            motor_set_phase(i, motor_step[i]);

            if(counter < 10) {
                delay(5);
            } else {
                delay(2);
            }

            motor_step[i]--;
            counter++;
        }

        motor_step[i] = 0;
        motor_speed[i] = MOTOR_DEF_SPEED;
        motor_off(i);

        Serial.println("Init axe end!");
    }
    
    Serial.println("Init end!");
}

int8_t check_buttons(uint8_t num) {
  if(num > 1)
    return 0;

  if(digitalRead(BUTTON_PINS[num][0])) {
    return -1;
  }
  if(digitalRead(BUTTON_PINS[num][1])) {
    return 1;
  }

  return 0;
}

int16_t motor_set_step(uint8_t num, int16_t step) {
  if(num > 1 || step == 0)
    return 0;

  int8_t step_dur = (step > 0)? 1: -1;
  uint16_t counter = 0;

  while(step) {
    if(check_buttons(num) == step_dur) {
      Serial.println("BUT");
      break;
    }

    motor_step[num] += step_dur;  
    motor_set_phase(num, motor_step[num]);
    if(counter < 10) {
        delay(5);
    } else {
        // delay(1000 / motor_speed[num]);
        delayMicroseconds(1e6 / motor_speed[num]);
    }

    step -= step_dur;
    counter++;
  }

  motor_off(num);
  return step;
}

void motor_set_phase(uint8_t num, uint8_t step) {
  uint8_t new_phase = step % 4;
  for(uint8_t i = 0; i < 4; ++i) {
    digitalWrite(MOTOR_PINS[num][i], PHASE[new_phase][i]);
  }
}

void motor_off(uint8_t num) {
  for(uint8_t i = 0; i < 4; ++i) {
    digitalWrite(MOTOR_PINS[num][i], 0);
  }
}

void motor_zero(uint8_t num) {
    uint16_t counter = 0;

    while(check_buttons(num) != -1) {
        motor_set_phase(num, motor_step[num]);

        if(counter < 10) {
            delay(5);
        } else {
            delay(2);
        }

        motor_step[num]--;
        counter++;
    }

    motor_off(num);
}

void motor_set_speed(uint8_t num, uint16_t speed) {
    if(num > 1)
        return;
    
    if(speed < MOTOR_MAX_SPEED)
        motor_speed[num] = speed;
    else
        motor_speed[num] = MOTOR_MAX_SPEED;
}
