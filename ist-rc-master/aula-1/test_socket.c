#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(void) {
  int fd, n;
  struct sockaddr_in address;
  struct hostent *host;

  // Host.
  if ((host = gethostbyname("Pedros-MacBook-Pro.local")) == NULL) exit(1);

  // UDP Socket.
  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) exit(1);

  memset((void*)&address, (int) '\0', sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = *host->h_addr_list[0];
  address.sin_port = htons(58000);

  n = sendto(fd, "Hello!\n", 7, 0, (struct sockaddr*)&address, sizeof(address));
  if (n == -1) exit(1);
  exit(0);
}
