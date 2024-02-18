#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PORT 58000

int main(void) {
  int fd;
  struct hostent *hostptr;
  struct sockaddr_in serveraddr;

  fd = socket(AF_INET,SOCK_DGRAM, 0);
  hostptr = gethostbyname("localhost");

  memset((void*)&serveraddr, (int) '\0', sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = ((struct in_addr *)(hostptr->h_addr_list[0]))->s_addr;
  serveraddr.sin_port = htons((u_short) PORT);

  int command_line_len = 256;
  int size, n;
  socklen_t addrlen;
  char other_buff[100];
  char *command_line = (char*) calloc(sizeof(char), command_line_len);

  while (strcmp(command_line, "exit\n") != 0) {
    fgets(command_line, command_line_len, stdin);

    size = 0;
    while (command_line[size] != '\n') size++;
    size++;

    n = sendto(fd, command_line, size, 0, (struct sockaddr*) &serveraddr, sizeof(serveraddr));
    if (n == -1) exit(-1);

    memset(command_line, '\0', 256);


    addrlen = sizeof(serveraddr);
    n = recvfrom(fd, other_buff, sizeof(other_buff), 0, (struct sockaddr*) &serveraddr, &addrlen);
    other_buff[n] = '\0';
    memset(other_buff, '\0', 100);
  }

  close(fd);
}
