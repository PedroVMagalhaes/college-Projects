#include "user.h"

extern User *g_user;

User *create_User(const char *user_name, const char *password) {
  User *new_user = (User *) malloc(sizeof(User));
  new_user->username = strdup(user_name);
  new_user->password = strdup(password);

  return new_user;
}

void free_User(User *user) {
  if (user) {
    if (user->password) {
      free(user->password);
    }

    if (user->username) {
      free(user->username);
    }
  }
}

void wait_for_command(char *cs_name, u_int16_t *cs_port) {
  const int command_line_len = 256;
  int inst_array_len = 0;
  char *command_line, ***inst_array;

  command_line = (char*) calloc(command_line_len, sizeof(char));
  inst_array = (char ***) malloc(sizeof(char **));

  check_server_info(&cs_name, cs_port);

  /* CS hostent */
  struct hostent *cs_ent = gethostbyname(cs_name);

  /* CS interner address */
  struct sockaddr_in *server_addr = get_server_addr(AF_INET, *cs_port, cs_ent);

  while (TRUE) {
    fprintf(stdout, "> ");
    fgets(command_line, command_line_len, stdin);
    command_line[strlen(command_line) - 1] = '\0'; /* Exclude '\n' */

    inst_array_len = str_split(command_line, inst_array, DELIMITER);

    if (strcmp((*inst_array)[0], LOGIN_CMD) == 0) {
      if (inst_array_len == 3) {
        validate_user_name_syntax((*inst_array)[1]);
        validate_user_password_syntax((*inst_array)[2]);
      }

      g_user = create_User((*inst_array)[1], (*inst_array)[2]);
      int socket_fd = login(server_addr, TRUE);
      close(socket_fd);

    } else if (strcmp((*inst_array)[0], DELUSER_CMD) == 0) {
      delete_user(server_addr);

    } else if (strcmp((*inst_array)[0], BACKUP_CMD) == 0) {
      char *dir_path = get_real_path(2, ".", (*inst_array)[1]);
      backup_dir(server_addr, dir_path);
      free(dir_path);

    } else if (strcmp((*inst_array)[0], RESTORE_CMD) == 0) {
      char *dir_path = get_real_path(2, ".", (*inst_array)[1]);
      restore_dir(server_addr, dir_path);
      free(dir_path);

    } else if (strcmp((*inst_array)[0], DIRLIST_CMD) == 0) {
      dirlist(server_addr);

    } else if (strcmp((*inst_array)[0], FILELIST_CMD) == 0) {
      char *dir_path = get_real_path(2, ".", (*inst_array)[1]);
      filelist_dir(server_addr, dir_path);
      free(dir_path);

    } else if (strcmp((*inst_array)[0], DELETE_CMD) == 0) {
      delete_dir(server_addr, (*inst_array)[1]);

    } else if (strcmp((*inst_array)[0], LOGOUT_CMD) == 0) {
      free_User(g_user);
      g_user = NULL;

    } else if (strcmp(command_line, EXIT_CMD) == 0) {
      break;

    } else {
      fprintf(stdout, "unknown command\n\n");
    }

    free_array(inst_array, inst_array_len);
  }
}

void send_protocol_message(const int tcp_or_udp, int socket_fd,
                           struct sockaddr *addr, socklen_t s_len,
                           const char *default_protocol_mess,
                           const int command_code, int args_num, ...) {

  va_list arg_list;
  char *temp_protocol_mess = strdup(default_protocol_mess);
  char *prot_part = strtok(temp_protocol_mess, DELIMITER);
  int first_time = TRUE;

  va_start(arg_list, args_num);

  while (prot_part != NULL) {
    if (first_time) {
      first_time = FALSE;

    } else {
      send_udp_or_tcp_message(tcp_or_udp, socket_fd, (void *) DELIMITER, DELIMITER_LEN, addr, s_len);
    }

    if (strcmp(prot_part, "%CODE") == 0) {
      char *code = get_command(command_code);
      send_udp_or_tcp_message(tcp_or_udp, socket_fd, (void *) code, strlen(code), addr, s_len);

    } else if (strcmp(prot_part, "%s") == 0) {
      char *new_arg = va_arg(arg_list, char *);
      send_udp_or_tcp_message(tcp_or_udp, socket_fd, (void *) new_arg, strlen(new_arg), addr, s_len);

    } else if (strcmp(prot_part, "%d") == 0) {
      int new_arg = va_arg(arg_list, int);
      char num_str[get_number_of_digits(new_arg) + 1];
      sprintf(num_str, "%d", new_arg);

      send_udp_or_tcp_message(tcp_or_udp, socket_fd, (void *) num_str, strlen(num_str), addr, s_len);

    } else if (strcmp(prot_part, "%DIR") == 0) {
      char *new_arg = va_arg(arg_list, char *);
      char *dir_name = basename(new_arg);
      send_udp_or_tcp_message(tcp_or_udp, socket_fd, (void *) dir_name, strlen(dir_name), addr, s_len);

    } else if (strcmp(prot_part, "%FILES") == 0) {
      FileInfo **files_info = va_arg(arg_list, FileInfo **);
      int n_files = va_arg(arg_list, int);

      print_files_and_data(TCP_SOCKET, socket_fd,NULL , 0, files_info, n_files, FALSE, DELIMITER,
                           DELIMITER_LEN);

    } else if (strcmp(prot_part, "%FILES_DATA") == 0) {
      FileInfo **files_info = va_arg(arg_list, FileInfo **);
      int n_files = va_arg(arg_list, int);

      print_files_and_data(TCP_SOCKET, socket_fd,NULL , 0, files_info, n_files, TRUE, DELIMITER,
                           DELIMITER_LEN);

    } else { /* Send 'prot_part' content */
      send_udp_or_tcp_message(tcp_or_udp, socket_fd, (void *) prot_part, strlen(prot_part), addr, s_len);
    }

    prot_part = strtok(NULL, DELIMITER);
  }

  send_udp_or_tcp_message(tcp_or_udp, socket_fd, (void *) "\n", 1, addr, s_len);
  free(temp_protocol_mess);
}

ProtocolObj *receive_protocol_message(const int udp_or_tcp, int socket_fd,
                                      struct sockaddr *addr, socklen_t s_len,
                                      const char *expected_message,
                                      const int command_code, const char *dir_path) {

  ProtocolObj *p_obj = (ProtocolObj *) malloc(sizeof(ProtocolObj));
  char *temp_protocol_mess = strdup(expected_message);
  char *expected_token = strtok(temp_protocol_mess, DELIMITER);

  char **server_tokens;
  int n_read = 0;

  while (expected_token != NULL) {
    if (strcmp(expected_token, "%CODE") == 0) {
      server_tokens = get_n_tokens_from_file(TCP_SOCKET, socket_fd, addr, &s_len, 1, DELIMITER);
      int rec_cmd = check_command(server_tokens[0]);
      free_array(&server_tokens, 1);

      if (rec_cmd != command_code || rec_cmd == ERR_COMMAND) {
        p_obj->_status = S_ERR;
        return p_obj;
      }

    } else if (strcmp(expected_token, "%STATUS") == 0) {
      server_tokens = get_n_tokens_from_file(TCP_SOCKET, socket_fd, addr, &s_len, 1, DELIMITER);

      p_obj->_status = check_status(command_code, server_tokens[0]);

      free_array(&server_tokens, 1);

    } else if (strcmp(expected_token, "%STATUS|ADDR_PORT") == 0) {
      server_tokens = get_n_tokens_from_file(TCP_SOCKET, socket_fd, addr, &s_len, 1, DELIMITER);
      p_obj->_status = check_status(command_code, server_tokens[0]);

      if (p_obj->_status == S_ERR || p_obj->_status == S_NOK ||
          p_obj->_status == S_EOF) {

        free_array(&server_tokens, 1);
        return p_obj;
      }

      p_obj -> _b_server_addr = strdup(server_tokens[0]);
      free_array(&server_tokens, 1);

      server_tokens = get_n_tokens_from_file(TCP_SOCKET, socket_fd, addr, &s_len, 1, DELIMITER);
      p_obj -> _b_server_port = (u_int16_t) atoi(server_tokens[0]);
      free_array(&server_tokens, 1);

    } else if (strcmp(expected_token, "%DIR") == 0) {
      p_obj -> _n_dirs = 1;
      p_obj -> _dirs = get_n_tokens_from_file(TCP_SOCKET, socket_fd, addr, &s_len, 1, DELIMITER);

    } else if (strcmp(expected_token, "%DIRS") == 0) {
      p_obj -> _n_dirs = n_read;

      p_obj -> _dirs = get_n_tokens_from_file(TCP_SOCKET, socket_fd, addr, &s_len,
                                                 p_obj -> _n_dirs,
                                                 DELIMITER);

    } else if (strcmp(expected_token, "%STATUS|d") == 0) {
      server_tokens = get_n_tokens_from_file(TCP_SOCKET, socket_fd, addr, &s_len, 1, DELIMITER);
      p_obj->_status = check_status(command_code, server_tokens[0]);

      if (p_obj->_status != S_NOT_STATUS) {
        free_array(&server_tokens, 1);
        return p_obj;
      }

      n_read = atoi(server_tokens[0]);
      free_array(&server_tokens, 1);

    } else if (strcmp(expected_token, "%d") == 0) {
      server_tokens = get_n_tokens_from_file(TCP_SOCKET, socket_fd, addr, &s_len, 1, DELIMITER);

      n_read = atoi(server_tokens[0]);

      free_array(&server_tokens, 1);

    } else if (strcmp(expected_token, "%FILES") == 0) {
      p_obj -> _n_files = n_read;

      get_n_file_info_from_file(TCP_SOCKET, socket_fd, addr, s_len, DELIMITER, dir_path,
                                &(p_obj->_files_info), p_obj->_n_files, FALSE);

    } else if (strcmp(expected_token, "%FILES_DATA") == 0) {
      p_obj -> _n_files = n_read;
      get_n_file_info_from_file(TCP_SOCKET, socket_fd, addr, s_len, DELIMITER, dir_path,
                                &(p_obj)->_files_info, p_obj->_n_files, TRUE);

    } else {
      fprintf(stderr, "Protocol message: wrong syntax\n");
    }

    expected_token = strtok(NULL, DELIMITER);
  }

  free(temp_protocol_mess);
  return p_obj;
}

int login(const struct sockaddr_in *cs_addr, const int first_login) {
  ProtocolObj *p_obj;

  int socket_fd = connect_to_server(TCP, AF_INET, cs_addr);
  send_protocol_message(TCP_SOCKET, socket_fd, NULL , 0, AUT_MESS, AUT_COMMAND, 2, g_user->username,
                        g_user->password);

   p_obj = receive_protocol_message(TCP_SOCKET, socket_fd, NULL, 0, AUR_MESS, AUR_COMMAND, NULL);

  if (first_login) {
    print_login_message(p_obj->_status);
  }

  free(p_obj);
  return socket_fd;
}

void print_login_message(const int status) {
  if (is_err_status(status)) return;

  char *status_str;
  switch (status) {
    case S_OK:
      status_str = "succedd";
      break;

    case S_NOK:
      status_str = "failed";
      break;

    case S_NEW:
      status_str = "and sign up succedd";
      break;
  }

  fprintf(stdout, "user '%s' login %s\n\n", g_user->username, status_str);
}

void delete_user(const struct sockaddr_in *cs_addr) {
  if (check_login()) {
    ProtocolObj *p_obj;

    int socket_fd = login(cs_addr, FALSE);
    send_protocol_message(TCP_SOCKET, socket_fd, NULL , 0, DLU_MESS, DLU_COMMAND, 0);
    p_obj = receive_protocol_message(TCP_SOCKET, socket_fd, NULL, 0, DLR_MESS, DLR_COMMAND, NULL);

    close(socket_fd);

    print_deluser_message(p_obj -> _status);

    free(p_obj);
  }

}

void print_deluser_message(const int status) {
  if (is_err_status(status)) return;

  char *status_str;
  switch (status) {
    case S_OK:
      status_str = "succedd";
      break;

    case S_NOK:
      status_str = "failed (delete all directories before proceed with this"
                   "operation";
      break;
  }

  fprintf(stdout, "user '%s' deletion %s\n\n", g_user->username, status_str);
}

void backup_dir(const struct sockaddr_in *cs_addr, char *dir_path) {
  if (check_login()) {
    int socket_fd;
    ProtocolObj *p_obj;

    int f_info_len = 0;
    FileInfo ***f_info = (FileInfo ***) malloc(sizeof(FileInfo **));
    get_files_info_in_dir(dir_path, f_info, &f_info_len);

    /*Connect to CS */
    socket_fd = login(cs_addr, FALSE);
    send_protocol_message(TCP_SOCKET, socket_fd, NULL , 0, BCK_MESS, BCK_COMMAND, 4, dir_path,
                          f_info_len, *f_info, f_info_len);

    p_obj = receive_protocol_message(TCP_SOCKET, socket_fd, NULL, 0, BKR_MESS, BKR_COMMAND,
                                     dir_path);

    close(socket_fd);

    /* Connect to BS */
    if (p_obj->_status != S_NOK && p_obj->_status != S_ERR &&
        p_obj->_status != S_EOF) {

      struct sockaddr_in *bs_addr;
      bs_addr = get_server_addr(AF_INET, p_obj -> _b_server_port,
                                gethostbyname(p_obj -> _b_server_addr));

      socket_fd = login(bs_addr, FALSE);

      send_protocol_message(TCP_SOCKET, socket_fd, NULL , 0, UPL_MESS, UPL_COMMAND, 4, dir_path,
                            p_obj->_n_files, p_obj->_files_info,
                            p_obj->_n_files);

      free(p_obj);
      p_obj = receive_protocol_message(TCP_SOCKET, socket_fd, NULL, 0, UPR_MESS, UPR_COMMAND, NULL);

      close(socket_fd);
    }

    print_backup_message(p_obj->_status, p_obj->_b_server_addr,
                         p_obj->_b_server_port, dir_path, p_obj->_files_info,
                         p_obj->_n_files);

    free(p_obj);
  }
}

void print_backup_message(const int status,
                          char *server_addr, const u_int16_t server_port,
                          char *dir_path, FileInfo **f_info,
                          int n_files) {

  if (is_err_status(status)) return;

  char *status_str;
  if (status == S_OK || status == S_NOT_STATUS) {
    status_str = "succedd";
  }

  switch (status) {
    case S_EOF:
      status_str = "failed (request could not be answered)";
      break;

    case S_ERR:
      status_str = "failed (request was not formulated correctly)";
      break;
  }

  fprintf(stdout, "'%s/':\n", basename(dir_path));
  fprintf(stdout, "\t");
  print_files_and_data(TCP_SOCKET, 1, NULL, 0, f_info, n_files, FALSE, "\n\t", 2);

  char *server_name = server_addr != NULL ? server_addr : "address unknown";
  char *port_str, *unk_port = "port unknown", n_port[8];
  port_str = unk_port;
  if (server_port != 0) {
    sprintf(n_port, "%hu", server_port);
    port_str = n_port;
  }

  fprintf(stdout, "\nbackup to: %s %s %s\n\n", server_name, port_str,
          status_str);
}

void restore_dir(const struct sockaddr_in *cs_addr, char *dir_path) {
  if (check_login()) {
    int socket_fd;
    ProtocolObj *p_obj;
    struct sockaddr_in *bs_addr;

    /* Connect to CS */
    socket_fd = login(cs_addr, FALSE);
    send_protocol_message(TCP_SOCKET, socket_fd, NULL , 0, RST_MESS, RST_COMMAND, 1, dir_path);
    p_obj = receive_protocol_message(TCP_SOCKET, socket_fd, NULL, 0, RSR_MESS, RSR_COMMAND, NULL);
    close(socket_fd);

    /* Connect to BS */
    if (p_obj->_status != S_NOK && p_obj->_status != S_ERR &&
        p_obj->_status != S_EOF) {

      /* Create dir if does not exist */
      struct stat st;
      if (stat(dir_path, &st) == -1) {
        mkdir(dir_path, 0777);
      }

      bs_addr = get_server_addr(AF_INET,p_obj->_b_server_port,
                                gethostbyname(p_obj->_b_server_addr));

      socket_fd = login(bs_addr, FALSE);
      send_protocol_message(TCP_SOCKET, socket_fd, NULL , 0, RSB_MESS, RSB_COMMAND, 1, dir_path);

      free(p_obj);
      p_obj = receive_protocol_message(TCP_SOCKET, socket_fd, NULL, 0, RBR_MESS, RBR_COMMAND,
                                      dir_path);

      close(socket_fd);
    }

    print_restoredir_message(p_obj->_status, p_obj->_b_server_addr,
                             p_obj->_b_server_port, dir_path,
                             p_obj->_files_info, p_obj->_n_files);

    free(p_obj);
  }
}

void print_restoredir_message(const int status,
                              char *server_addr, const u_int16_t server_port,
                              char *dir_path, FileInfo **f_info,
                              int n_files) {

  if (is_err_status(status)) return;

  char *status_str;
  if (status == S_OK || status == S_NOT_STATUS) {
    status_str = "succedd";
  }

  switch (status) {
    case S_EOF:
      status_str = "failed (request could not be answered)";
      break;

    case S_ERR:
      status_str = "failed (request was not formulated correctly)";
      break;
  }

  fprintf(stdout, "'%s/':\n", basename(dir_path));
  print_files_and_data(TCP_SOCKET, 1, NULL , 0, f_info, n_files, FALSE, "\n", 1);

  char *server_name = server_addr != NULL ? server_addr : "address unknown";
  char *port_str, *unk_port = "port unknown", n_port[10];

  port_str = unk_port;
  if (server_port != 0) {
    sprintf(n_port, "%hu", server_port);
    port_str = n_port;
  }

  fprintf(stdout, "\nrestore from: %s %s %s\n\n", server_name, port_str,
          status_str);
}

void dirlist(const struct sockaddr_in *cs_addr) {
  if (check_login()) {
    int socket_fd;
    ProtocolObj *p_obj;

    socket_fd = login(cs_addr, FALSE);
    send_protocol_message(TCP_SOCKET, socket_fd, NULL , 0, LSD_MESS, LSD_COMMAND, 0);
    p_obj = receive_protocol_message(TCP_SOCKET, socket_fd, NULL, 0, LDR_MESS, LDR_COMMAND, NULL);

    close(socket_fd);

    print_dirlist_message(p_obj->_status, p_obj->_dirs, p_obj->_n_dirs);

    free(p_obj);
  }
}

void print_dirlist_message(const int status, char **dirs, const int n_dirs) {
  if (is_err_status(status)) return;

  for (int i = 0; i < n_dirs; i++) {
    fprintf(stdout, "'%s/'\n", dirs[i]);
  }
  fprintf(stdout, "\n");
}

void filelist_dir(const struct sockaddr_in *cs_addr, char *dir_path) {
  if (check_login()) {
    int socket_fd;
    ProtocolObj *p_obj;

    socket_fd = login(cs_addr, FALSE);
    send_protocol_message(TCP_SOCKET, socket_fd, NULL , 0, LSF_MESS, LSF_COMMAND, 1, dir_path);
    p_obj = receive_protocol_message(TCP_SOCKET, socket_fd, NULL, 0, LFD_MESS, LFD_COMMAND,
                                     dir_path);

    close(socket_fd);

    print_filelist_dir_message(p_obj->_status, dir_path, p_obj->_files_info,
                               p_obj->_n_files);

    free(p_obj);
  }
}

void print_filelist_dir_message(const int status, char *dir_path,
                                FileInfo **f_info, const int n_files) {

  if (is_err_status(status)) return;

  if (status == S_OK || status == S_NOT_STATUS) {
    fprintf(stdout, "'%s/':\n", basename(dir_path));
    print_files_and_data(TCP_SOCKET, 1, NULL , 0, f_info, n_files, FALSE, "\n", 1);
  }

  switch (status) {
    case S_NOK:
      fprintf(stdout, "request could not be answered\n");
      break;
  }

  fprintf(stdout, "\n");
}

void delete_dir(const struct sockaddr_in *cs_addr, const char *dir_name) {
  if (check_login()) {
    int socket_fd;
    ProtocolObj *p_obj;

    socket_fd = login(cs_addr, FALSE);
    send_protocol_message(TCP_SOCKET, socket_fd, NULL , 0, DEL_MESS, DEL_COMMAND, 1, dir_name);
    p_obj = receive_protocol_message(TCP_SOCKET, socket_fd, NULL, 0, DDR_MESS, DDR_COMMAND, NULL);

    close(socket_fd);

    print_delete_dir_message(p_obj->_status, dir_name, p_obj->_files_info,
                             p_obj->_n_files);

    free(p_obj);
  }
}

void print_delete_dir_message(const int status, const char *dir_name,
                              FileInfo **f_info, const int n_files) {

  if (is_err_status(status)) return;

  fprintf(stdout, "'%s/':\n\t", dir_name);
  print_files_and_data(TCP_SOCKET, 1, NULL , 0, f_info, n_files, FALSE, " - deleted\n\t", 12);
}

int check_login() {
  if (g_user == NULL) {
    fprintf(stdout, "< %s\n\n", LOGIN_FIRST);
    return FALSE;
  }

  return TRUE;
}

void validate_user_name_syntax(const char *user_name) {
  const char dig_cont[1][2] = {{'0', '9'}};
  int error = check_string_len(user_name, MAX_USER_LEN);

  if (error == NO_ERROR) {
    error = check_string_chars(user_name, MAX_USER_LEN, dig_cont, 1);
  }

  if (error != NO_ERROR) {
    fprintf(stderr, "'user name': %s %s.\n", get_default_error_message(error),
            error == STRING_HAS_INVALID_CHARACTERS ?
              "(only numeric characters are permitted)" :
              ""
           );

    exit(-1);
  }
}

void validate_user_password_syntax(const char *password) {
  const char cont[3][2] = {{'0', '9'}, {'a', 'z'}, {'A', 'Z'}};
  int error = check_string_len(password, MAX_PASSWORD_LEN);

  if (error == NO_ERROR) {
    error = check_string_chars(password, MAX_PASSWORD_LEN, cont, 3);
  }

  if (error != NO_ERROR) {
    fprintf(stderr, "'password': %s %s.", get_default_error_message(error),
            error == STRING_HAS_INVALID_CHARACTERS ?
              "(only alphanumeric characters are permitted)" :
              ""
           );

    exit(-1);
  }
}

void check_server_info(char **cs_name, u_int16_t *cs_port) {
  /*  cs_name was not provided (CS will be running on the user machine) */
  if (*cs_name == NULL) {
    int cs_name_len = 256;

    *cs_name = (char*) calloc(cs_name_len, sizeof(char));
    gethostname(*cs_name, cs_name_len);
  }

  if (*cs_port == 0) {
    *cs_port = DEFAULT_PORT; /* Default port */
  }
}

int is_err_status(int status) {
  if (status == S_ERR) {
    fprintf(stdout, "unexpected protocol message recieved\n\n");
    return TRUE;
  }

  return FALSE;
}
