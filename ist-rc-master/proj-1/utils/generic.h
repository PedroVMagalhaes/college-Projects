#ifndef __UTILS_H__
#define __UTILS_H__

#define _GNU_SOURCE
#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <getopt.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <stdarg.h>
#include <time.h>
#include <utime.h>

#define FALSE 0
#define TRUE 1

#define LOGIN_CMD "login"
#define AUT_MESS "%CODE %s %s"
#define AUR_MESS "%CODE %STATUS"

#define DELUSER_CMD "deluser"
#define DLU_MESS "%CODE"
#define DLR_MESS "%CODE %STATUS"

#define BACKUP_CMD "backup"
#define BCK_MESS "%CODE %DIR %d %FILES"
#define BKR_MESS "%CODE %STATUS|ADDR_PORT %d %FILES"

#define RESTORE_CMD "restore"
#define RST_MESS "%CODE %DIR"
#define RSR_MESS "%CODE %STATUS|ADDR_PORT"

#define DIRLIST_CMD "dirlist"
#define LSD_MESS "%CODE"
#define LDR_MESS "%CODE %d %DIRS"

#define FILELIST_CMD "filelist"
#define LSF_MESS "%CODE %DIR"
#define LFD_MESS "%CODE %STATUS|ADDR_PORT %d %FILES"

#define DELETE_CMD "delete"
#define DEL_MESS "%CODE %DIR"
#define DDR_MESS "%CODE %STATUS"

#define UPL_MESS "%CODE %DIR %d %FILES_DATA"
#define UPR_MESS "%CODE %STATUS"

#define RSB_MESS "%CODE %DIR"
#define RBR_MESS "%CODE %STATUS|d %FILES_DATA"

#define LOGOUT_CMD "logout"
#define EXIT_CMD "exit"

#define DELIMITER_LEN 1
#define DELIMITER " "
#define END_CHAR "\n"

#define DEFAULT_PORT (u_int16_t) 58034
#define FILENAME_SIZE 23
#define NO_ERROR 0

#define DATE_TIME_FORMAT_LEN 20
#define DATE_TIME_FORMAT "%d.%m.%Y %H:%M:%S"

#define MAX_FILES 20
#define FILE_INFO "%s %s %d\n"

typedef struct {
  char *_path;
  time_t _date_time;
  int _size;
} FileInfo;

typedef struct bs_node_t {
  char *id;
  struct bs_node_t *next;
} BS_Node;

typedef struct BS_List_t {
  int count;
  struct bs_node_t *first;
} BS_List;

typedef struct backup_record_t {
  char **dirs;
  char *bs_info;
  int count;
  struct backup_record_t *next;
} Record;

typedef struct backup_record_list_t {
  int count;
  struct backup_record_t *first;
} Record_List;

typedef struct user_t {
  char *username;
  char *password;
  char *filename;
} User;

enum PROT_COMMAND {
  AUT_COMMAND = 0,
  AUR_COMMAND = 1,
  DLU_COMMAND = 2,
  DLR_COMMAND = 3,
  BCK_COMMAND = 4,
  BKR_COMMAND = 5,
  RST_COMMAND = 6,
  RSR_COMMAND = 7,
  LSD_COMMAND = 8,
  LDR_COMMAND = 9,
  LSF_COMMAND = 10,
  LFD_COMMAND = 11,
  DEL_COMMAND = 12,
  DDR_COMMAND = 13,
  UPL_COMMAND = 14,
  UPR_COMMAND = 15,
  RSB_COMMAND = 16,
  RBR_COMMAND = 17,
  ERR_COMMAND = 18,
};

enum S_STATUS {
  S_FALSE = 0,
  S_OK = 1,
  S_NOK = 2,
  S_EOF = 3,
  S_ERR = 4,
  S_NEW = 5,
  S_NOT_STATUS = -100
};

enum ERRORS {
  STRING_LENGTH_IS_GREATER_THAN_MAX_STR_LEN = -10,
  STRING_HAS_INVALID_CHARACTERS = -20,
  CANNOT_PARSE_STRING_TO_INT = -30,
  INVALID_NUMBER_OF_ARGS = -40,
  INVALID_OPT_ARG_SYNTAX = -50,
  DUPLICATE_OPT_ARG = -60,
  NO_USER = -100,
  WRONG_PASSWORD = -101,
  EXIT_NO_USER_TCP_CONNECTION = -500,
  EXIT_USER_UNABLE_TO_FORK = -501,
  ERROR_WHEN_WRITTING_IN_FILE = -502,
  ERROR_WHEN_READING_FROM_FILE = -503,
  ERROR_WHEN_READ_INFO_ABOUT_FILE = -504,
  CANNOT_CONVERT_STR_TO_TIME_T = -505,
  CANNOT_OPEN_DIR = -506,
  CANNOT_CLOSE_DIR = -507
};

enum SEND_TO_SOCKET {
  TCP_SOCKET,
  UDP_SOCKET
};

/**
 * Parse string into an unsigned integer.
 *
 * @author Group 34
 * @param  str  Pointer to first char of the string.
 * @param  name Pointer to first char of variable name.
 * @return Parsed value.
 */
unsigned short int parse_ushort_or_exit(char const *str, char const *name);


/* Copy 'str' to the heap
 * @author group 34
 * @param str - string to be copied
 * @return pointer to the new string
 */
char *strdup(const char *str);

int get_number_of_digits(int number);
char *nstrdup(char **dest, int *dest_len, int n_str, ...);
int strtok_count(char *str, const char *delim);

/*
 * Check how many tokens are in 'str' when its divided by 'delim' string
 * @author group 34
 * @param str - string to be divided
 * @param delim - delimiter string
 * @return unsigned int - number of tokens found
 */
int str_split(char *str, char ***split_array, const char *delim);

int check_string_len_and_chars(
  const char *str, const unsigned int str_len,
  const char ascii_cont[][2],
  const unsigned int ascii_cont_len
);
char *str_join(char *str1, char *str2, const char *glue);
char *array_join(char ***arr, int arr_size, const char *glue);
void free_array(char ***arr, int size);
void display_error(int err_code);


/*
 * Divide 'commmand' by 'delim' string and put all the tokens in 'inst_array'
 * (2D array)
 * @author group 34
 * @param command - string to be divided
 * @param command_len - length of 'command'
 * @param delim - delimiter string
 * @param inst_array - 2D array to put all tokens found in 'command'
 * @param inst_array_len - length of 'inst_array'
 * @return 0 - if no error occured
 */
int parse_command(char *command, int command_len, const char *delim,
                  char ***inst_array, int *inst_array_len);

/*
 * Complete default protocol message to send to CS or BS.
 * @author group 34
 * @param default_mess - Default protocol message without specific information
 *                       about some server.
 * @param opt_args - (2D array) all specific information to be added to the
 *                   default message before sending to the server
 * @return 0 - if no error occured
 */
void get_protocol_message(const char *default_mess, char *out_mess,
                          int *out_mess_len, int opt_args_num, ...);

/*
 * Check if 'str' length is less than 'max_str_len'.
 * @author group 34
 * @param str - string to be checked
 * @param max_str_len - Max length possible for 'str'. If 'str' length is greater
 *                       than 'max_str_len' send an error and exit
 * @return NO_ERROR - if no error was found.
 *          negative number - Error number
 *                            (STRING_LENGTH_IS_GREATER_THAN_MAX_STR_LEN)
 *                            (based on 'ERRORS' enum)
 */
int check_string_len(const char *str, const unsigned int max_str_len);

/*
 * Check if 'str' length is less than 'max_len' and if all 'str'
 *           ASCII code characters are between 'ascii_cont'. (Example of
 *           ascii_cont const char letter_cont[2][2] = {{'a', 'z'},
 *           {'A', 'Z'}})
 * @author group 34
 * @param str - string to be checked
 * @param ascii_cont - Matrix with a variable number of lines and with 2
 *                      columns. Each line represents a container, where the
 *                      first column has the minimum ascii code possible for
 *                      a character belongs to that container and the second
 *                      column has the maximum ascii char possible.
 *                      (Example const char letter_cont = {{'a', 'z'},
 *                      {'A', 'Z'}})
 * @return 0 - if no error was encoutered;
 *          negative number - Error number (STRING_HAS_INVALID_CHARACTERS)
 *                            (based on 'ERRORS' enum)
 */
int check_string_chars(const char *str, const unsigned int max_str_len,
                       const char ascii_cont[][2],
                       const unsigned int ascii_cont_len);

int is_str_in_container(const char **container, const int container_len,
                        const char *str);

/*
 * Send a default message based on the error number to the terminal
 * @author group 34
 * @param error_numer - Error number (based on 'ERRORS' enum)
 */
const char *get_default_error_message(const int error_number);

/* Fill 'server_addr' info with the server info pointed by 'server_ent'
 * @author group 34
 * @param domain - specifies a communication domain(this selects the protocol
 *                  family which will be used for communication; e.g. AF_INET)
 * @param server_ent - information about the server to connect
 * @return struct sockaddr_in - pointer to structure to deal with internet
 *                              adresses (to connect to server by a socket)
 */
struct sockaddr_in *get_server_addr(const int domain, const u_int16_t server_port,
                                    struct hostent *server_ent);

/*
 * Connect to a server by its host name
 * @author group 34
 * @param c_protocol - Connection protocol to be used (TCP or UDP)
 * @param domain - specifies a communication domain(this selects the protocol
 *                  family which will be used for communication; e.g. AF_INET)
 * @param server_name - Server's host name the we want to establish a
 *                       connection
 * @param server_port CS port to connect
 * @return int - positive number if the file descriptor of the socket file;
 *                negative number is the error code encountered (based on
 *                'sys/socket.h' errors)
 */
int connect_to_server_by_name(const int c_protocol, const int domain,
                              const char *server_name, const uint16_t port);

/*
 * Connect to a server
 * @author group 34
 * @param c_protocol - Connection protocol to be used (TCP or UDP)
 * @param domain - specifies a communication domain(this selects the protocol
 *                  family which will be used for communication; e.g. af_inet)
 * @param server_ent - Pointer to the server hostent structure
 * @param server_port - CS port to connect
 * @return int - positive number if the file descriptor of the socket file;
 *                negative number is the error code encountered (based on
 *                'sys/socket.h' errors)
 */
int connect_to_server(const int c_protocol, const int domain,
                      const struct sockaddr_in *server_addr);

#endif
