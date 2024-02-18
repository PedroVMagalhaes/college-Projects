#ifndef __CS_TO_BS_H__
#define __CS_TO_BS_H__

#include "../utils/generic.h"
#include "../utils/coms.h"
#include "../utils/bs_list.h"
#include "../utils/file_utils.h"
#include "../utils/command_utils.h"

#define REG_CMD "REG"
#define UNR_CMD "UNR"

/**
 * Start UDP server to handle BS registrations.
 *
 * @author Group 34
 * @param  port Port number to start the UDP server on.
 */
void bs_handler(unsigned short int port);

char *bs_exec_command(char ***command, int size);
int bs_create_user(char *bs_info);
int bs_list_files(char *bs_info, char *dir, FileInfo ***files);
int bs_delete_dir(char *bs_info, char *dir);

#endif
