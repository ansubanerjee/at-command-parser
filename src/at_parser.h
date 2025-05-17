#ifndef AT_PARSER_H
#define AT_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

void at_parser_init(void);

void at_parser_process_char(char c);

typedef void (*command_callback_t)(const char *cmd);

void at_parser_set_callback(command_callback_t cb);

#ifdef __cplusplus
}
#endif

#endif // AT_PARSER_H
