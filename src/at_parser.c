#include "at_parser.h"
#include <string.h>
#include <stdio.h>

#define CMD_BUFFER_SIZE 128

static char cmd_buffer[CMD_BUFFER_SIZE];
static int cmdIndex = 0;
static int overflow = 0;

static command_callback_t command_callback = NULL;

void at_parser_set_callback(command_callback_t cb) {
    command_callback = cb;
}

void at_parser_init(void) {
    cmdIndex = 0;
    overflow = 0;
    memset(cmd_buffer, 0, CMD_BUFFER_SIZE);
}

void at_parser_process_char(char c) {
    if (c == '\r' || c == '\n') {
        if (overflow) {
            overflow = 0;
            cmdIndex = 0;
            printf("ERROR: Command too long\r\n");
            return;
        }
        if (cmdIndex > 0) {
            cmd_buffer[cmdIndex] = '\0';
            if (command_callback) {
                command_callback(cmd_buffer);  
            }
            cmdIndex = 0;
        }
    }
    else if (overflow) {
        return;
    }
    else if (cmdIndex < CMD_BUFFER_SIZE - 1) {
        cmd_buffer[cmdIndex++] = c;
    }
    else {
        overflow = 1;
    }
}
