#include "bs_to_cs.h"

extern char *g_data_dir;
extern struct sockaddr_in g_cs_addr;
extern User *g_user;

int register_with_cs(unsigned short int port) {
  int fd;
  char msg[128];
  char host[128];
  char *response = NULL;
  char **parsed_response;
  int complete = TRUE;

  if (gethostname(host, 128) == -1) {
    fprintf(stderr, "Error determining BS hostname.\n");
    exit(-1);
  }

  if (sprintf(msg, "REG %s %hu\n", host, port) == -1) {
    fprintf(stderr, "Error building register message.\n");
    exit(-1);
  }

  fd = open_udp_socket();

  // Try to register to CS.
  if (udp_send(fd, &g_cs_addr, msg) == -1) complete = FALSE;
  else if (udp_receive(fd, &g_cs_addr, &response) == -1) complete = FALSE;

  if (complete) {
    str_split(response, &parsed_response, " ");
    if (strcmp(parsed_response[1], "OK") != 0)
      complete = FALSE;
  }

  // TODO: Deregister on exit_flag.

  close(fd);

  return complete;
}

void cs_handler(unsigned short int port) {
  int udp_fd; /* UDP socket file descriptor */
  struct sockaddr_in addr; /* Socket address */
  socklen_t addrlen;

  // Initialize the UDP server.
  addr = config_addr("", port);
  udp_fd = start_udp_server(addr);

  // TODO: cleanup this communication model.
  while (TRUE) {
    addrlen = sizeof(addr);
    cs_exec_command(udp_fd, addr, addrlen);
  }

  // TODO: Add ^C signal handling with a flag to reach here.
  close(udp_fd);
}


void cs_exec_command(int fd, struct sockaddr_in addr, socklen_t s_len) {
  int result;
  char **tokens = get_n_tokens_from_file(UDP_SOCKET, fd, (struct sockaddr*) &addr, &s_len, 1, DELIMITER);

  // List files for directory.
  if (strcmp(tokens[0], LSF_CMD) == 0) {
    free_array(&tokens, 1);

    tokens = get_n_tokens_from_file(UDP_SOCKET, fd, (struct sockaddr*) &addr, &s_len, 2, DELIMITER);
    char *dir_path = get_real_path(4, ".", g_data_dir, tokens[0], tokens[1]);

    send_files(UDP_SOCKET, fd, (struct sockaddr*) &addr, s_len, dir_path);

    free(dir_path);
    free_array(&tokens, 2);

  // Register a user with a backup server.
  } else if (strcmp(tokens[0], LSU_CMD) == 0) {
    char *folder;
    result = verify_user(tokens[1], tokens[2]);

    if (result == NO_USER) {

      // Create file to reference user.
      if (create_user(tokens[1], tokens[2]) == -1) send_udp_or_tcp_message(UDP_SOCKET, fd, "LUR ERR\n", 8, (struct sockaddr*) &addr, s_len);

      // Create folder.
      folder = str_join(g_data_dir, tokens[1], "/");
      mkdir(folder, 0770);
      free(folder);
      send_udp_or_tcp_message(UDP_SOCKET, fd, "LUR OK\n", 7, (struct sockaddr*) &addr, s_len);
    }
    send_udp_or_tcp_message(UDP_SOCKET, fd, "LUR NOK\n", 8, (struct sockaddr*) &addr, s_len);

    free_array(&tokens, 3);

  // Delete backed up directory.
  } else if (strcmp(tokens[0], DLB_CMD) == 0) {
    char *u_folder;
    char *folder;
    // TODO: handle the fact that the BS does not knoe the password for the current user.
    u_folder = str_join(g_data_dir, tokens[1], "/");
    folder = str_join(u_folder, tokens[2], "/");
    if (remove(folder) != -1) {
      // Delete user if this was his last backed up folder.
      if (rmdir(u_folder) == 0) unlink(g_user->filename);
      send_udp_or_tcp_message(UDP_SOCKET, fd, "DBR OK\n", 7, (struct sockaddr*) &addr, s_len);
    }
    send_udp_or_tcp_message(UDP_SOCKET, fd, "DBR NOK\n", 8, (struct sockaddr*) &addr, s_len);

    free_array(&tokens, 3);
  }

  send_udp_or_tcp_message(UDP_SOCKET, fd, "ERR\n", 4, (struct sockaddr*) &addr, s_len);
}
