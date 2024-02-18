#ifndef COMMAND_UTILS_H
#define COMMAND_UTILS_H

#include "generic.h"

int check_status(const int command, const char *status);

int check_command(const char *command);

int parse_status(const char *status);

char *get_command(const int command_code);

#endif
