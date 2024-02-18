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

  fd = socket(AF_INET,SOCK_STREAM,0);
  hostptr = gethostbyname("localhost");

  memset((void*)&serveraddr, (int) '\0', sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = ((struct in_addr *)(hostptr->h_addr_list[0]))->s_addr;
  serveraddr.sin_port = htons((u_short) PORT);

  connect(fd,(struct sockaddr*)&serveraddr, sizeof(serveraddr));

  int command_line_len = 256;
  int size;
  char *command_line = (char*) calloc(sizeof(char), command_line_len);

  while (strcmp(command_line, "exit") != 0) {
    fgets(command_line, command_line_len, stdin);

    size = 0;
    while (command_line[size] != '\n') size++;
    size++;

    char other_buff[10];
    write(fd, command_line, size);
    read(fd, other_buff, 10);
  }

  close(fd);
}
