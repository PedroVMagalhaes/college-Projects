#include "cs_to_bs.h"

extern BS_List *bs_list;
extern User *g_user;
extern char *g_data_dir;
extern int errno;

void bs_handler(unsigned short int port) {
  int udp_fd; /* UDP socket file descriptor */
  struct sockaddr_in addr; /* Socket address */
  socklen_t addrlen;
  char **cmd;
  int retry_count;
  int cmd_size;
  char *response;
  char buffer[128];
  char *cmd_line; /* Reveive buffer */

  // Initialize the UDP server.
  addr = config_addr("", port);
  udp_fd = start_udp_server(addr);

  // TODO: cleanup this communication model.
  while (TRUE) {
    addrlen = sizeof(addr);

    response = "";
    cmd_line = "";
    memset(buffer, '\0', sizeof(buffer));

    while (recvfrom(udp_fd, buffer, sizeof(buffer), 0, (struct sockaddr*) &addr, &addrlen) > 0) {
      cmd_line = str_join(cmd_line, buffer, "");
      memset(buffer, '\0', sizeof(buffer));
      if (strstr(cmd_line, "\n") == NULL) continue;
      break;
    }

    printf("CMD: %s\n", cmd_line);
    cmd_size = str_split(cmd_line, &cmd, " ");
    response = bs_exec_command(&cmd, cmd_size);

    retry_count = 0;
    while (
      retry_count < 3 &&
      sendto(udp_fd, response, strlen(response), 0, (struct sockaddr*) &addr, sizeof(addr)) == -1
    ) {
      retry_count++;
    }

    printf("end loop transmission\n");
    free(cmd_line);
  }

  // TODO: Add ^C signal handling with a flag to reach here.
  close(udp_fd);
}


char *bs_exec_command(char ***command, int size) {
  char **cmd = (*command);
  int result;

  printf("execute: %s, %d\n", cmd[0], size);
  // Register backup server.
  if (strcmp(cmd[0], REG_CMD) == 0) {
    // TODO: Validate server connection.
    if (size != 3) return strdup("RGR ERR\n");
    char *bs_id = str_join(get_host_ip(cmd[1]), cmd[2], " ");
    result = add_bs(bs_id);
    free(bs_id);
    if (result == FALSE) return strdup("RGR NOK\n");
    return strdup("RGR OK\n");

  // Unregister backup server.
  } else if (strcmp(cmd[0], UNR_CMD) == 0) {
    if (size != 3) return strdup("UAR ERR\n");
    char *bs_id = str_join(cmd[1], cmd[2], " ");
    result = remove_bs(bs_id);
    free(bs_id);
    if (result == FALSE) return strdup("UAR NOK\n");
    return strdup("UAR OK\n");
  }

  return strdup("ERR\n");
}


// TODO: abstract the sending of a command.
int bs_create_user(char *bs_info) {
  struct sockaddr_in *addr;
  int fd;
  char **bs_data;
  char response[128];
  char *cmd = "LSU";
  char **parsed_response;
  socklen_t addrlen;

  // Get BS data.
  char *aux_bs_info = strdup(bs_info);
  str_split(aux_bs_info, &bs_data, " ");
  addr = get_server_addr(AF_INET, (u_int16_t) atoi(bs_data[1]), gethostbyname(bs_data[0]));

  addrlen = sizeof(addr);
  printf("BS Info: %s %s\n", bs_data[0], bs_data[1]);
  fd = open_udp_socket(addr);
  free_array(&bs_data, 2);
  free(aux_bs_info);

  printf("after open\n");
  printf("%d\n", g_user==NULL);
  printf("%s\n", g_user->username);
  printf("%s\n", g_user->password);

  // Build and send cmd.
  cmd = str_join(cmd, g_user->username, " ");
  cmd = str_join(cmd, g_user->password, " ");
  cmd = str_join(cmd, "\n", "");
  printf("%s\n", cmd);
  if (send_udp_or_tcp_message(UDP_SOCKET, fd, cmd, strlen(cmd), (struct sockaddr*) addr, sizeof(*addr)) == -1) {
    printf("errno: %d\n", errno);
    return -1;
  }
  printf("Something in between\n");
  if (receive_udp_or_tcp_message(UDP_SOCKET, fd, response, sizeof(response), (struct sockaddr*) addr, &addrlen) == -1) {
  printf("errno: %d\n", errno);
    return -1;
  }
  printf("After receive on bs_create\n");

  str_split(response, &parsed_response, " ");
  if (strcmp(parsed_response[1], "OK") != 0) return -1;
  return 1;
}


int bs_list_files(char *bs_info, char *dir, FileInfo ***files) {
  int fd;
  char **bs_data;

  str_split(strdup(bs_info), &bs_data, DELIMITER);
  struct sockaddr_in *addr = get_server_addr(AF_INET, (u_int16_t) atoi(bs_data[1]), gethostbyname(bs_data[0]));

  int lsf_mess_len = 0;
  char **lsf_mess = (char **) malloc(sizeof(char *));
  nstrdup(lsf_mess, &lsf_mess_len, 5, "LSF ", g_user->username, " ", dir, "\n");

  fd = open_udp_socket(addr);

  send_udp_or_tcp_message(UDP_SOCKET, fd, (void *) *lsf_mess, lsf_mess_len,
                          (struct sockaddr *) &addr, sizeof(addr));

  char **tokens = get_n_tokens_from_file(TCP_SOCKET, fd, NULL, NULL, 2, DELIMITER);
  int error = get_n_file_info_from_file(TCP_SOCKET, fd, NULL, 0, DELIMITER, dir, files, atoi(tokens[1]), FALSE);
  return atoi(tokens[1]);
}


int bs_delete_dir(char *bs_info, char *dir) {
  char *msg;
  struct sockaddr_in addr;
  int fd;
  char **bs_data;
  char *response;
  char **parsed_response;

  // Get BS data.
  str_split(bs_info, &bs_data, " ");
  addr = config_addr(
    bs_data[0],
    parse_ushort_or_exit(bs_data[1], "bs_create_user port")
  );
  fd = open_udp_socket(addr);
  free_array(&bs_data, 2);

  msg = str_join("DLB", g_user->username, " ");
  msg = str_join(msg, dir, " ");
  if (udp_send(fd, &addr, msg) == -1) return -1;

  // Receive and process response.
  if (udp_receive(fd, &addr, &response) == -1) return -1;
  str_split(response, &parsed_response, " ");
  if (strcmp(parsed_response[1], "OK") != 0) return -1;
  return 1;
}
