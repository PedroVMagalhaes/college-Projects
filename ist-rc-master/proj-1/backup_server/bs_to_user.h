#ifndef __BS_TO_USER_H__
#define __BS_TO_USER_H__

#include "../user_client/user.h"
#include "../utils/generic.h"
#include "../utils/coms.h"
#include "../utils/auth.h"
#include "../utils/file_utils.h"
#include "../utils/command_utils.h"

// Commands
#define AUT_CMD "AUT"
#define UPL_CMD "UPL"
#define RSB_CMD "RSB"

#define DELIMITER_LEN 1
#define DELIMITER " "

int user_server(unsigned short int port);
void user_handle_connection(int fd);
void handle_protocol_message(int fd);
char *user_exec_command(char **cmd, int cmd_size);

#endif
