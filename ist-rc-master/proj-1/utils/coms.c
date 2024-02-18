#include "coms.h"

char *get_host_ip(char *hostname) {
  struct hostent *hostptr;
  hostptr = gethostbyname(hostname);
  return inet_ntoa(*((struct in_addr*)(hostptr->h_addr_list[0])));
}

struct sockaddr_in config_addr(char *hostname, unsigned short int port) {
  struct sockaddr_in addr; /* Socket address */
  // Socket configuration.
  memset((void*)&addr, (int) '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons((u_short) port);
  if (strcmp(hostname, "") == 0)
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
  else {
    struct hostent *hostptr;
    hostptr = gethostbyname(hostname);
    addr.sin_addr.s_addr = ((struct in_addr*)(hostptr->h_addr_list[0]))->s_addr;
  }

  return addr;
}


int open_tcp_socket() {
  int socket_fd; /* Socket file descriptor */

  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr, "Error creating TCP socket.\n");
    exit(-1);
  }

  return socket_fd;
}


int open_udp_socket() {
  int socket_fd; /* Socket file descriptor */

  if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    fprintf(stderr, "Error creating TCP socket.\n");
    exit(-1);
  }

  return socket_fd;
}


int start_tcp_server(struct sockaddr_in addr) {
  int socket_fd = open_tcp_socket();

  // Bind to the configured address and port.
  if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    fprintf(stderr, "Error binding TCP server socket.\n");
    exit(-1);
  }

  if (listen(socket_fd, 5) == -1) {
    fprintf(stderr, "Error listening on TCP socket.\n");
    exit(-1);
  }

  return socket_fd;
}



int start_udp_server(struct sockaddr_in addr) {
  int socket_fd; /* Socket file descriptor */

  // Open socket.
  if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    fprintf(stderr, "Error creating UDP socket.\n");
    exit(-1);
  }

  // Bind to the configured address and port.
  if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    fprintf(stderr, "Error binding UDP server socket.\n");
    exit(-1);
  }

  return socket_fd;
}


int udp_send(int fd, struct sockaddr_in *addr, char *msg) {
  int retry_count;

  retry_count = 0;
  while (
    retry_count < 3 &&
    sendto(fd, msg, strlen(msg), 0, (struct sockaddr*) addr, sizeof(*addr)) == -1
  ) {
    retry_count++;
  }

  if (retry_count == 3) return -1;
  return 1;
}


int udp_receive(int fd, struct sockaddr_in *addr, char **response) {
  char buffer[128]; /* Reveive buffer */
  char *last_char;
  socklen_t addrlen = sizeof(*addr);

  *response = "";
  memset(buffer, '\0', sizeof(buffer));

  while (recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr*) addr, &addrlen) > 0) {
    *response = str_join(*response, buffer, "");
    memset(buffer, '\0', sizeof(buffer));
    if (strstr(*response, "\n") == NULL) continue;
    break;
  }

  if ((last_char = strstr(*response, "\n")) == NULL) return -1;
  *last_char = '\0';
  return 1;
}


int tcp_send(int fd, char *response) {
  int n; /* Number of bytes to send */
  int nw; /* Number of bytes sent */
  char *ptr; /* Reveive buffer position */

  ptr = &response[0];
  n = strlen(response);
  nw = 0;
  while (n > 0) {
    if ((nw = write(fd, ptr, n)) <= 0) return -1;
    n -= nw;
    ptr += nw;
  }

  return 1;
}


int tcp_receive(int fd, char **cmd_line) {
  int n; /* Number of bytes read */
  char buffer[128]; /* Reveive buffer */
  char *last_char;
  *cmd_line = "";

  memset(buffer, '\0', sizeof(buffer));
  while ((n = read(fd, buffer, sizeof(buffer))) != 0) {
    if (n == -1) {
      fprintf(stderr, "Error reading from user TCP socket.\n");
      return -1;
    }
    *cmd_line = str_join(*cmd_line, buffer, "");
    memset(buffer, '\0', sizeof(buffer));
    if ((last_char = strstr(*cmd_line, "\n")) == NULL) continue;
    *last_char = '\0';
    break;
  }

  return 1;
}
