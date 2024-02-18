#ifndef USER_H
#define USER_H

#include "../utils/generic.h"
#include "../utils/command_utils.h"
#include "../utils/file_utils.h"

#define MAX_USER_LEN 5
#define MAX_PASSWORD_LEN 8

#define LOGIN_FIRST "please, login first before using any other command"

#define DEFAULT_PORT (u_int16_t) 58034
#define TCP SOCK_STREAM

typedef struct {
  int _status;
  char **_dirs;
  int _n_dirs;
  FileInfo **_files_info;
  int _n_files;
  char *_b_server_addr;
  u_int16_t _b_server_port;
} ProtocolObj;

#define READ_BUFFER_LEN 128
char *read_buffer;
int read_buffer_pointer;

User *create_User(const char *user_name, const char *password);

void free_User(User *user);

/*
 * User interaction. Read a predefined command from terminal and execute it
 * @author group 34
 * @param cs_name - Name of the central server
 * @param cs_port_str - Port of the central server to connect
 */
void wait_for_command(char *cs_name, u_int16_t *cs_port);

void send_protocol_message(const int tcp_or_udp, int socket_fd,
                           struct sockaddr *addr, socklen_t s_len,
                           const char *default_protocol_mess,
                           const int command_code, int args_num, ...);

ProtocolObj *receive_protocol_message(const int udp_or_tcp, int socket_fd,
                                      struct sockaddr *addr, socklen_t s_len,
                                      const char *expected_message,
                                      const int command_code, const char *dir_path);

int check_status(const int command, const char *status);

/*
 * Login new user to the server
 * @authorg group 34
 * @param user_name - IST number of the user (1 up to 5 digits)
 * @param password - User password composed by 8 alphanumerical characters
 * @return int - 0 if the User Application couldn't authenticate the user;
 *                Other value is the authentication was successful
 */
int login(const struct sockaddr_in *server_addr, const int first_login);

void print_login_message(const int status);

void delete_user(const struct sockaddr_in *server_addr);

void print_deluser_message(const int status);

void backup_dir(const struct sockaddr_in *server_addr, char *dir_name);

void print_backup_message(const int status,
                          char *server_addr, const u_int16_t server_port,
                          char *dir_name, FileInfo **f_info, int n_files);

void restore_dir(const struct sockaddr_in *server_addr, char *dir_name);

void print_restoredir_message(const int status,
                              char *server_addr, const u_int16_t server_port,
                              char *dir_name, FileInfo **f_info, int n_files);

void dirlist(const struct sockaddr_in *server_addr);

void print_dirlist_message(const int status, char **dirs, const int n_dirs);

void filelist_dir(const struct sockaddr_in *server_addr, char *dir_name);

void print_filelist_dir_message(const int status, char *dir_name,
                                FileInfo **f_info, const int n_files);

void delete_dir(const struct sockaddr_in *server_addr, const char *dir_name);

void print_delete_dir_message(const int status, const char *dir_name,
                              FileInfo **f_info, const int n_files);

int check_login();

/*
 * Check if the user name is valid (1 up to 5 digits)
 * @author group 34
 * @param user_name - User IST number (1 up to 5 digits)
 */
void validate_user_name_syntax(const char *user_name);

/*
 * Check if the user password is valid
 * @author group 34
 * @param password - User password composed by 8 alphanumerical characters
 */
void validate_user_password_syntax(const char *password);

/*
 * Check if 'cs_name' is NULL, and if so 'cs_name' will store the hostname of
 * the current processor. If 'cs_port' is NULL then 'cs_port' will store the
 * port number 58034
 * @author group 34
 * @param cs_name - Name of the server to connect
 * @param cs_port - Port of the server to connect
 * @return int - 0 if no error occured;
 *               negative number if an error occured
 */
void check_server_info(char **cs_name, u_int16_t *cs_port);

int is_err_status(int status);

#endif
