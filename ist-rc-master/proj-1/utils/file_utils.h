#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "../user_client/user.h"
#include "generic.h"

#define FILE_TOKENS 4

char **get_n_tokens_from_file(const int tcp_or_udp, int from_fd,
                              struct sockaddr *addr, socklen_t *s_len,
                              int n_tokens, const char *delim);

int get_n_file_info_from_file(const int tcp_or_udp, const int file_fd,
                              struct sockaddr *addr, socklen_t s_len,
                              const char *delim, const char *dir_path,
                              FileInfo ***files, const int n_files,
                              const int parse_data);

char *file_info_to_str(const FileInfo *f_info);

/*
 * Parse information about the file to the formatted string: "$FILE_NAME
 * dd.mm.yyy hh:mm:ss $FILE_SIZE"
 * @author group 34
 * @param f - 'f's struct dirent
 * @param formatted_file - pointer to the formatted string
 * @param formatted_file_len - Length of the formatted string
 * @return int - 0 if no error occured;
 *               negative number if an error occured
 */
int get_file_info(char *file_path, FileInfo **f_info);

/*
 * Parse information about all files in a directory to an array string. Each
 * element have a formatted string about a file, which is: "$FILE_NAME
 * dd.mm.yyy hh:mm:ss $FILE_SIZE"
 * @author group 34
 * @param dir_path - directory's path
 * @param formatted_array - Array to put all files' formatted info
 * @param formatted_el_len - Array containing the length for each string of
 *                           'formatted_array'
 * @return int - 0 if no error occured;
 *               negative number if an error occured
 */
int get_files_info_in_dir(const char *dir_path, FileInfo ***files_info,
                           int *files_info_len);


int send_content_between_files(const int from_tcp_or_udp, int from_fd,
                               const int to_tcp_or_udp, int to_fd, int size,
                               struct sockaddr *addr, socklen_t s_len);

time_t parse_str_to_time(const char *time_str, const char *format);

int change_last_mod_time_of_file(const char *file_name, time_t new_mod_time);

void print_files_and_data(const int udp_or_tcp, const int to_fd,
                          struct sockaddr *addr, socklen_t s_len,
                          FileInfo **files_info, const int files_info_len,
                          const int print_data, const char *delimiter,
                          const int delimiter_len);

char *get_real_path(int n_paths, ...);

int send_udp_or_tcp_message(int tcp_or_udp, int fd, void *buff, int buff_len,
                             struct sockaddr *addr, socklen_t len);

int receive_udp_or_tcp_message(int tcp_or_udp, int fd, void *buff, int buff_len,
                             struct sockaddr *addr, socklen_t *len);

void copy_files(const int tcp_or_udp, int fd, struct sockaddr *addr, socklen_t s_len, char *dir_path, int n_files);
void send_files(const int tcp_or_udp, int fd, struct sockaddr *addr,
                socklen_t s_len, char *dir_path);

#endif
