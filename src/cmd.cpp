
#include "main.h"
#include "cmd.h"
#include "motors.h"

static char identity[]="Positioner interface v. 1.0 (C) Sidorov (Ambro), 2026";

static uint8_t mesg[128] = {0};
static uint8_t mesg_len = 0;

// --------------------------------------------------------------------------
static void cmd_identity(char *args, void *context);
static void cmd_read(char *args, void *context);
static void cmd_init(char *args, void *context);
static void cmd_button(char *args, void *context);
static void cmd_set(char *args, void *context);
static void cmd_zero(char *args, void *context);
static void cmd_speed(char *args, void *context);
static void cmd_help(char *args, void *context);

Cmd_t commands[CMD_COUNT] = {
  {"*IDN?",       cmd_identity,     NULL,            5,     "Get board identity"},
  {"READ?",       cmd_read,         NULL,            5,     "Read state"},
  {"INIT",        cmd_init,         NULL,            4,     "Init motors"},
  {"BUTTON",      cmd_button,       NULL,            6,     "Print button state"},
  {"SETX",        cmd_set,          (void *) STP_X,  4,     "Set X"},
  {"SETY",        cmd_set,          (void *) STP_Y,  4,     "Set Y"},
  {"ZEROX",       cmd_zero,         (void *) STP_X,  5,     "Set 0 X"},
  {"ZEROY",       cmd_zero,         (void *) STP_Y,  5,     "Set 0 Y"},        
  {"SPEEDX",      cmd_speed,        (void *) STP_X,  6,     "Set speed X in step/sec (500 MAX)"},
  {"SPEEDY",      cmd_speed,        (void *) STP_Y,  6,     "Set speed Y in step/sec (500 MAX)"},
  {"HELP",        cmd_help,         NULL,            4,     "Print help"}
};

void process_cmd(char *cmd) {
    char *cmd_ptr;
	char *cmd_token;

	cmd_token = strtok_r((char*)cmd, "\r\n", &cmd_ptr);

	if (!cmd_token)
		return;

	while (cmd_token) {
		uint8_t cmd_found = 0;
        uint8_t cmd_len = strcspn(cmd_token, " \t");

        for(uint8_t i = 0; i < CMD_COUNT; ++i) {
            if(cmd_len == commands[i].cmd_len && !strncmp(cmd_token, commands[i].text_cmd, commands[i].cmd_len)) {
                if(commands[i].handler) {
                    commands[i].handler(cmd_token + cmd_len, commands[i].context);
                }
                cmd_found = 1;
                break;
            }
			
		}

		if (!cmd_found) {
			mesg_len = snprintf((char*)mesg, 128, "Unknown command: %s\r\n", cmd_token);
			Serial.write(mesg, mesg_len);
		}

		cmd_token = strtok_r(NULL, "\r\n", &cmd_ptr);
	}
}

// --------------------------------------------------------------------------
static void cmd_identity(char *args, void *context) {
  Serial.println(identity);
}

// --------------------------------------------------------------------------
static void cmd_read(char *args, void *context) {
    mesg_len = sprintf((char*)mesg, "X= %d (%d), Y= %d (%d)\r\n", 
            motor_step[0], motor_speed[0], motor_step[1], motor_speed[1]);
    Serial.write(mesg, mesg_len);
}

// --------------------------------------------------------------------------
static void cmd_init(char *args, void *context) {
    motor_init();
}

// --------------------------------------------------------------------------
static void cmd_button(char *args, void *context) {
    mesg_len = sprintf((char*)mesg, "Btn_X= %d, Btn_Y= %d\r\n", 
            check_buttons(0), check_buttons(1));
    Serial.write(mesg, mesg_len);
}

// --------------------------------------------------------------------------
static void cmd_set(char *args, void *context)
{
    uint8_t stp_index = (int)context;
    char *endptr;
  
    int16_t value = 0;

	if(!args[0]) {
        Serial.println("**Error**: Invalid argument!");
		return;
    }

	value = strtol(args, &endptr, 10);
	if(endptr == args) {
		Serial.println("**Error**: Invalid argument!");
        return;
	}

    mesg_len = sprintf((char*)mesg, "Set %c step = %d (%d)\r\n", 
            (stp_index == 0)? 'X': 'Y', value, motor_speed[stp_index]);
    Serial.write(mesg, mesg_len);

    motor_set_step(stp_index, value);
}

void cmd_zero(char *args, void *context) {
  int stp_index = (int) context;
  motor_zero(stp_index);
}

// --------------------------------------------------------------------------
void cmd_speed(char *args, void *context) {
    uint8_t stp_index = (int)context;
    char *endptr;
  
    int16_t value = 0;

	if(!args[0]) {
        Serial.println("**Error**: Invalid argument!");
		return;
    }

	value = strtol(args, &endptr, 10);
	if(endptr == args) {
		Serial.println("**Error**: Invalid argument!");
        return;
	}

    mesg_len = sprintf((char*)mesg, "Set %c speed = %d\r\n", 
            (stp_index == 0)? 'X': 'Y', value);
    Serial.write(mesg, mesg_len);

    motor_set_speed(stp_index, value); 
}

// --------------------------------------------------------------------------
#define HELP_INDENT 15
static void cmd_help(char *args, void *context)
{
  int spaces;
  Serial.println("");
  Serial.println(identity);
  if ((args != NULL) && (*args != 0))
  {
    for (int i = 0; i < CMD_COUNT; i++)
    {
      if (! strncmp(args, commands[i].text_cmd, strlen(commands[i].text_cmd)))
      {
        Serial.print(commands[i].text_cmd);
        spaces = HELP_INDENT - strlen(commands[i].text_cmd);
        if (spaces < 2) spaces = 2;
        for (int j = 0; j < spaces; j++)
        {
          Serial.print(" ");
        }
        Serial.println(commands[i].text_help);
        break;
      }
    }
  } else
  {
    for (int i = 0; i < CMD_COUNT; i++)
    {
        Serial.print(commands[i].text_cmd);
        spaces = HELP_INDENT - strlen(commands[i].text_cmd);
        if (spaces < 2) spaces = 2;
        for (int j = 0; j < spaces; j++)
        {
          Serial.print(" ");
        }
        Serial.println(commands[i].text_help);
    }
  }
}

