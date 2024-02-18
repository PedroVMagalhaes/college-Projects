#ifndef __CS_TO_USER_H__
#define __CS_TO_USER_H__

#include <sys/types.h>
#include <unistd.h>

#include "../user_client/user.h"
#include "../utils/generic.h"
#include "../utils/bs_list.h"
#include "../utils/coms.h"
#include "../utils/auth.h"
#include "../utils/file_utils.h"

// Commands
#define AUT_CMD "AUT"
#define DLU_CMD "DLU"
#define BCK_CMD "BCK"
#define RST_CMD "RST"
#define LSD_CMD "LSD"
#define LSF_CMD "LSF"
#define DEL_CMD "DEL"

/**
 * Start TCP server to handle BS registrations.
 * The server creates a child process on each accpeted connection, making it
 * concurrent.
 *
 * @author Group 34
 * @param  port Port number to start the TCP server on.
 */
int user_server(unsigned short int port);

void user_handle_connection(int fd);
void handle_protocol_message(int fd);
char *user_exec_command(char **cmd, int cmd_size);
int user_read_cmd(int fd, char **cmd_line);
int user_write_response(int fd, char *response);
User *get_existing_user(char *username);
void load_records();
void load_record(char *dir, char *bs_info, Record *prev);
void delete_record(Record *record);
char *list_dir();
char *get_bs_info(char *dir);
char *get_random_bs();
int check_bs_availability(char *id);
int dir_has_backup(char *dir);
 int diff(FileInfo ***files, FileInfo ***u_files, int u_files_len, FileInfo ***b_files, int b_files_len);

#endif
