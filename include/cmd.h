#ifndef INC_CMD_H_
#define INC_CMD_H_

#include <Arduino.h>

#define CMD_COUNT 11

#define STP_X 0
#define STP_Y 1

typedef struct Cmd_t
{
  const char *text_cmd;
  void (*handler)(char *, void *);
  void *context;
  uint8_t cmd_len;
  const char *text_help;
} Cmd_t;

void process_cmd(char *cmd);


#endif