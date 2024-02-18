#include "file_utils.h"
#include "../user_client/user.h"

extern User *g_user;
extern char *g_data_dir;

char **get_n_tokens_from_file(const int tcp_or_udp, int from_fd,
                              struct sockaddr *addr, socklen_t *s_len,
                              int n_tokens, const char *delim) {

  char **out_tokens = (char **) calloc(n_tokens, sizeof(char *));
  int out_tokens_len = 0;

  int read_buffer_len = 128;
  char c, read_buffer[read_buffer_len + 1];

  for (int i = 0; i < n_tokens; i++) {
    memset((void *) read_buffer, '\0', read_buffer_len);
    int j = 0;

    while (TRUE) {
      receive_udp_or_tcp_message(tcp_or_udp, from_fd, (void *) &c, sizeof(char), addr, s_len);

      if (c == ' ' || c == '\n') {
        break;
      }

      read_buffer[j++] = c;

      if (j == read_buffer_len + 1) {
        nstrdup(&(out_tokens[i]), &out_tokens_len, 2, out_tokens[i], read_buffer);
        memset((void *) read_buffer, '\0', read_buffer_len);
        j = 0;
      }
    }

    nstrdup(&(out_tokens[i]), &out_tokens_len, 2, out_tokens[i], read_buffer);
  }

  return out_tokens;
}

int get_n_file_info_from_file(const int tcp_or_udp, const int file_fd,
                              struct sockaddr *addr, socklen_t s_len,
                              const char *delim, const char *dir_path,
                              FileInfo ***files, const int n_files,
                              const int parse_data) {

  int date_time_str_len = 0, error;
  char **info_tokens, **date_time_str = (char **) malloc(sizeof(char *));
  *files = (FileInfo **) calloc(n_files, sizeof(FileInfo *));

  for (int i = 0; i < n_files; i++) {
    info_tokens = get_n_tokens_from_file(tcp_or_udp, file_fd, addr, &s_len, FILE_TOKENS, delim);

    FileInfo *f_info = (FileInfo *) malloc(sizeof(FileInfo));
    f_info -> _path = get_real_path(2, dir_path, info_tokens[0]);

    nstrdup(date_time_str, &date_time_str_len, 3, info_tokens[1], " ", info_tokens[2]);
    f_info -> _date_time = parse_str_to_time(*date_time_str, DATE_TIME_FORMAT);
    free(info_tokens[1]);
    free(info_tokens[2]);

    f_info -> _size = atoi(info_tokens[3]);
    free(info_tokens[3]);

    if (parse_data) {
      int to_fd = open(f_info -> _path, O_WRONLY | O_CREAT, S_IRWXO | S_IRWXG | S_IRWXU);

      error = send_content_between_files(tcp_or_udp, file_fd, TCP_SOCKET, to_fd, f_info -> _size, addr, s_len);
      if (error != NO_ERROR) {
        return error;
      }
      close(to_fd);

      error = change_last_mod_time_of_file(f_info -> _path, f_info -> _date_time);
      if (error != NO_ERROR) {
        return error;
      }

      char a;
      read(file_fd, (void *) &a, sizeof(char)); /* Read delimiter after data */
    }

    (*files)[i] = f_info;
  }

  return NO_ERROR;
}

char *file_info_to_str(const FileInfo *f_info) {
  char *f_name = strdup(basename(f_info -> _path));

  int file_str_len = strlen(f_name);
  file_str_len += 1 + DATE_TIME_FORMAT_LEN + 1 +
                    get_number_of_digits(f_info -> _size) + 1;

  char **file_str = (char **) malloc(sizeof(char *));

  char date_time[DATE_TIME_FORMAT_LEN + 1];
  strftime(
    date_time,
    DATE_TIME_FORMAT_LEN,
    DATE_TIME_FORMAT,
    localtime(&(f_info -> _date_time))
  );

  char size_str[get_number_of_digits(f_info -> _size) + 1];
  sprintf(size_str, "%d", f_info -> _size);
  nstrdup(file_str, &file_str_len, 5, f_name, " ", date_time, " ",
          size_str);

  char *f_str = *file_str;
  free(file_str);
  return f_str;
}


int get_file_info(char *file_path, FileInfo **f_info) {
  *f_info = (FileInfo *) malloc(sizeof(FileInfo));
  struct stat f_stat;

  (*f_info)->_path = strdup(file_path);

  if (stat(file_path, &f_stat) == -1) {
    return ERROR_WHEN_READ_INFO_ABOUT_FILE;
  }

  (*f_info)->_date_time = f_stat.st_mtime;
  (*f_info)->_size = (int) f_stat.st_size;

  return NO_ERROR;
}


int get_files_info_in_dir(
  const char *dir_path,
  FileInfo ***files_info,
  int *files_info_len
) {

  DIR *dir;
  struct dirent *dir_file;

  dir = opendir(dir_path);
  if (dir == NULL) {
    return CANNOT_OPEN_DIR;
  }

  *files_info_len = 0;

  while ((dir_file = readdir(dir)) != NULL) {
    if (dir_file -> d_type == DT_REG) {
      (*files_info_len)++;
    }
  }

  if (closedir(dir) == -1) {
    return CANNOT_CLOSE_DIR;
  }

  *files_info = (FileInfo **) calloc(*files_info_len, sizeof(FileInfo *));

  dir = opendir(dir_path);
  if (dir == NULL) {
    return CANNOT_OPEN_DIR;
  }

  int i = 0;
  while ((dir_file = readdir(dir)) != NULL)  {
    if (dir_file -> d_type == DT_REG) {
      char *f_path = get_real_path(2, dir_path, dir_file -> d_name);

      int error = get_file_info(f_path, &((*files_info)[i++]));
      if (error != NO_ERROR) {
        free(f_path);
        return error;
      }

      free(f_path);
    }
  }


  if (closedir(dir) == -1) {
    return CANNOT_CLOSE_DIR;
  }

  return NO_ERROR;
}

int send_content_between_files(const int from_tcp_or_udp, int from_fd,
                               const int to_tcp_or_udp, int to_fd, int size,
                               struct sockaddr *addr, socklen_t s_len) {

  int buffer_len = 128, n_bytes_read, n_bytes_wrote, left = size;
  void *buffer = calloc(buffer_len + 1, 1);

  do {
    memset(buffer, '\0', buffer_len + 1);

    int read_n_bytes = left < buffer_len ? left : buffer_len;
    n_bytes_read = receive_udp_or_tcp_message(from_tcp_or_udp, from_fd,
                                              buffer, read_n_bytes, addr,
                                              &s_len);

    left -= n_bytes_read;

    if (n_bytes_read < 0) {
      free(buffer);
      return ERROR_WHEN_READING_FROM_FILE;
    }

    n_bytes_wrote = send_udp_or_tcp_message(to_tcp_or_udp, to_fd, buffer,
                                            n_bytes_read, addr, s_len);

    if (n_bytes_wrote < 0 || n_bytes_wrote != n_bytes_read) {
      free(buffer);
      return ERROR_WHEN_WRITTING_IN_FILE;
    }

  } while (left > 0);

  free(buffer);
  return NO_ERROR;
}


time_t parse_str_to_time(const char *time_str, const char *format) {
  struct tm new_tm;
  char *time_str_processed = strptime(time_str, format, &new_tm);

  if (*time_str_processed != '\0') {
    return CANNOT_CONVERT_STR_TO_TIME_T;
  }

  time_t t = mktime(&new_tm);

  if (t == -1) {
    return CANNOT_CONVERT_STR_TO_TIME_T;
  }

  return t;
}


int change_last_mod_time_of_file(const char *file_name, time_t new_mod_time) {
  struct stat f_stat;

  if (stat(file_name, &f_stat) == -1) {
    return ERROR_WHEN_READ_INFO_ABOUT_FILE;
  }

  struct utimbuf new_times;
  new_times.actime = f_stat.st_atime;
  new_times.modtime = new_mod_time;

  return utime(file_name, &new_times);
}

void print_files_and_data(
  const int udp_or_tcp,
  const int to_fd,
  struct sockaddr *addr,
  socklen_t s_len,
  FileInfo **files_info,
  const int files_info_len,
  const int print_data,
  const char *delimiter,
  const int delimiter_len
) {

  for (int i = 0; i < files_info_len; i++) {
    char *file_str = file_info_to_str(files_info[i]);
    send_udp_or_tcp_message(udp_or_tcp, to_fd, (void *) file_str, strlen(file_str), addr, s_len);

    if (print_data == TRUE) {
      send_udp_or_tcp_message(udp_or_tcp, to_fd, (void *) " ", 1, addr, s_len);

      int from_fd = open(files_info[i] -> _path, O_RDONLY);
      send_content_between_files(TCP_SOCKET, from_fd, udp_or_tcp, to_fd,
                                 files_info[i] -> _size, addr, s_len);

      close(from_fd);
    }

    if (i != files_info_len - 1) {
      send_udp_or_tcp_message(udp_or_tcp, to_fd, (void *) delimiter, delimiter_len, addr, s_len);
    }
  }
}

char *get_real_path(int n_paths, ...) {
  va_list paths;
  int full_path_len = 0;
  char **full_path = (char **) malloc(sizeof(char *));

  va_start(paths, n_paths);
  for (int i = 0; i < n_paths; i++) {
    char *p = va_arg(paths, char *);

    if (i == 0) {
      *full_path = strdup(p);

    } else {
      char *old_full_path = *full_path;
      nstrdup(full_path, &full_path_len, 3, *full_path, "/", p);
      free(old_full_path);
    }
  }

  char *f_path = *full_path;
  free(full_path);
  return f_path;
}

int send_udp_or_tcp_message(int tcp_or_udp, int fd, void *buff, int buff_len,
                             struct sockaddr *addr, socklen_t len) {

  if (tcp_or_udp == TCP_SOCKET) {
    return write(fd, buff, buff_len);
  } else if (tcp_or_udp == UDP_SOCKET) {
    return sendto(fd, buff, buff_len, 0, addr, len);
  }

  return -1;
}

int receive_udp_or_tcp_message(int tcp_or_udp, int fd, void *buff, int buff_len,
                             struct sockaddr *addr, socklen_t *len) {

  if (tcp_or_udp == TCP_SOCKET) {
    return read(fd, buff, buff_len);
  } else if (tcp_or_udp == UDP_SOCKET) {
    return recvfrom(fd, buff, buff_len, 0, addr, len);
  }

  return -1;
}

void copy_files(const int tcp_or_udp, int fd, struct sockaddr *addr, socklen_t s_len, char *dir_path, int n_files) {
  /* Create dir if does not exist */
  struct stat st;
  if (stat(dir_path, &st) == -1)
    mkdir(dir_path, 0770);

  FileInfo ***files = (FileInfo ***) malloc(sizeof(FileInfo **));
  int error = get_n_file_info_from_file(tcp_or_udp, fd, addr, s_len, DELIMITER, dir_path, files,
                                        n_files, TRUE);

  if (error == NO_ERROR) {
    write(fd, (void *) "UPR OK\n", 7);
  } else {
    write(fd, (void *) "UPR NOK\n", 8);
  }

  free(files);
}

void send_files(const int tcp_or_udp, int fd, struct sockaddr *addr, socklen_t s_len, char *dir_path) {
  char **tokens = get_n_tokens_from_file(tcp_or_udp, fd, addr, &s_len, 1, DELIMITER);

  int f_info_len = 0;
  FileInfo ***f_info = (FileInfo ***) malloc(sizeof(FileInfo **));
  get_files_info_in_dir(dir_path, f_info, &f_info_len);

  send_protocol_message(tcp_or_udp, fd, addr, s_len, RBR_MESS, RBR_COMMAND, 3, f_info_len, *f_info,
                        f_info_len);

  free_array(&tokens, 1);
}
