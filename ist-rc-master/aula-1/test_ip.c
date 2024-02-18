#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(void) {
  struct hostent *host;
  struct in_addr *address;

  if ((host = gethostbyname("Pedros-MacBook-Pro.local")) == NULL) exit(1);

  printf("official host name: %s\n", host->h_name);

  address = (struct in_addr*) host->h_addr_list[0];
  printf("internet address: %s (%081X)\n", inet_ntoa(*address), ntohl(address->s_addr));
  exit(0);
}
