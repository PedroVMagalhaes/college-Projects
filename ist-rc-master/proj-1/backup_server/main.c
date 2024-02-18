#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>

#include "../utils/generic.h"
#include "bs_to_user.h"
#include "bs_to_cs.h"

BS_List *bs_list;
User *g_user;
char *g_data_dir = "bs_data";
struct sockaddr_in g_cs_addr;

int main(int argc, char **argv) {
  int c; /* Command option */
  int r_errno = 0;
  unsigned short int port; /* Port value for the servers */
  unsigned short int cs_port; /* Port value for the CS server. */
  char *cs_host;
  char *cvalue; /* Command port value */
  pid_t udp_server_pid; /* Child process ID */

  while ((c = getopt(argc, argv, "b:n:p:")) != -1) {
    switch (c) {
      case 'b':
        cvalue = optarg;
        if (cvalue != NULL) {
          port = parse_ushort_or_exit(cvalue, "BS port");
          break;
        }
        break;
      case 'n':
        cvalue = optarg;
        if (cvalue != NULL) cs_host = cvalue;
        break;
      case 'p':
        cvalue = optarg;
        if (cvalue != NULL) {
          cs_port = parse_ushort_or_exit(cvalue, "CS port");
          break;
        }
      default:
        abort();
    }
  }


  // Use default port if none is provided.
  if (argc < 3) {
    fprintf(stderr, "Command args not correct.\n");
    return -1;
  }

  // Make sure the CS data folder is present.
  mkdir(g_data_dir, 0770);

  // Build CS address.
  g_cs_addr = config_addr(cs_host, cs_port);

  // Register with CS server.
  if (register_with_cs(port) == FALSE) return -1;

  // Start UDP server for BS registration.
  if ((udp_server_pid = fork()) == -1) {
    fprintf(stderr, "Unable to execute UDP server start.\n");
    return -1;
  }

  if (udp_server_pid == 0) {
    cs_handler(port);
  } /* if child process */
  else {
    r_errno = user_server(port);
  } /* parent process */

  if (r_errno != 0) display_error(r_errno);
  return 1;
}
