#include "main.h"
#include "cmd.h"
#include "motors.h"

// static char mesg[64];
// static uint8_t mesg_len = 0;

static char rx_buf[64];
static uint8_t rx_buf_pos = 0;

static void pin_init(void);

void setup() {
  pin_init();
  Serial.begin(115200);
}

void loop() {
  if(Serial.available()) {
    rx_buf_pos = 0;
    delay(5);

    while(Serial.available()) {
      rx_buf[rx_buf_pos++] = Serial.read();
    }
    rx_buf[rx_buf_pos] = 0;

    process_cmd(rx_buf);
  }

  delay(10);
}

static void pin_init(void) {
  for(uint8_t i = 0; i < 2; ++i) {
    for(uint8_t j = 0; j < 2; ++j) {
      pinMode(BUTTON_PINS[i][j], INPUT_PULLUP);
    }
    for(uint8_t j = 0; j < 4; ++j) {
      pinMode(MOTOR_PINS[i][j], OUTPUT);
      digitalWrite(MOTOR_PINS[i][j], 0);
    }
  }
}



