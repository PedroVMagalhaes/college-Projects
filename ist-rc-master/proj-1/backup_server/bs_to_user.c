#include "bs_to_user.h"

extern User *g_user;
extern char *g_data_dir;
extern int errno;
int g_errno = 1;

int user_server(unsigned short int port) {
  int tcp_fd; /* TCP socket file descriptor */
  int user_fd; /* User client socket file descriptor */
  struct sockaddr_in addr; /* Socket address */
  socklen_t addrlen; /* Socket address size */
  pid_t user_pid;
  int ref;

  addr = config_addr("", port);
  tcp_fd = start_tcp_server(addr);

  addrlen = sizeof(addr);

  // TODO: add busy response, in case the the fork takes too long.
  while (g_errno == TRUE) {

    // Accept connections.
    do user_fd = accept(tcp_fd, (struct sockaddr*) &addr, &addrlen);
    while (user_fd == -1 && errno == EINTR);
    if (user_fd == -1) {
      close(user_fd);
      g_errno = EXIT_NO_USER_TCP_CONNECTION;
      continue;
    }

    // Fork to create child process to handle the connection concurrently.
    if ((user_pid = fork()) == -1) {
      g_errno = EXIT_USER_UNABLE_TO_FORK;
    }
    else if (user_pid == 0) {
      close(tcp_fd);
      printf("New Child\n");
      int i;
      for (i = 0; i < 2; i++) {
        printf("i: %d\n", i);
        handle_protocol_message(user_fd);
      }
    }

    printf("Socket closes\n");

    do ref = close(user_fd);
    while (ref == -1 && errno == EINTR);
    if (ref == -1) exit(-1);
  }

  close(tcp_fd);
  return g_errno;
}


void handle_protocol_message(int fd) {
  // Get code.
  char **tokens = get_n_tokens_from_file(TCP_SOCKET, fd, NULL, NULL, 1, DELIMITER);
  printf("%s\n", tokens[0]);

  // Call function to received 'code'.
  if (strcmp(tokens[0], AUT_CMD) == 0) {
    free_array(&tokens, 1);

    tokens = get_n_tokens_from_file(TCP_SOCKET, fd, NULL, NULL, 2, DELIMITER);
    printf("username: %s\npass: %s\n", tokens[0], tokens[1]);
    int result = verify_user(tokens[0], tokens[1]);
    printf("result: %d", result);
    if (result == NO_USER || result == WRONG_PASSWORD) {
      write(fd, (void *) "AUR NOK\n", 8);
    } else {
      load_user(tokens[0], tokens[1]);
      write(fd, "AUR OK\n", 7);
      printf("username: %s\npass: %s\n", g_user->username, g_user->password);
    }
    free_array(&tokens, 2);

  // Upload directory files.
  } else if (strcmp(tokens[0], UPL_CMD) == 0) {
    printf("get tokens\n");
    printf("user: %d\n", g_user == NULL);
    printf("data_dir: %s\n", g_data_dir);
    char **tokens = get_n_tokens_from_file(TCP_SOCKET, fd, NULL, NULL, 2, DELIMITER);
    printf("dir: %s; count: %s (size %lu)", tokens[0], tokens[1], strlen(tokens[1]));

    char *dir_path = get_real_path(3, g_data_dir, g_user->username, tokens[0]);
    printf("dirpath: %s\n", dir_path);
    copy_files(TCP_SOCKET, fd, NULL, 0, tokens[0], atoi(tokens[1]));
    free_array(&tokens, 2);

    //TODO: check error.

  // Restore directory files.
  } else if (strcmp(tokens[0], RSB_CMD) == 0) {
    free_array(&tokens, 1);

    tokens = get_n_tokens_from_file(TCP_SOCKET, fd, NULL, NULL, 1, DELIMITER);
    char *dir_path = get_real_path(4, ".", g_data_dir, g_user->username, tokens[0]);
    free_array(&tokens, 1);

    send_files(TCP_SOCKET, fd, NULL, 0, dir_path);
    free(dir_path);
    //TODO: check error.

  } else {
    printf("Error returned\n");
    write(fd, "ERR\n", 4);
  }

  free_array(&tokens, 1);
}
