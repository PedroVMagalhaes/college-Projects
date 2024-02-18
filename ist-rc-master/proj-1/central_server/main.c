#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>

#include "../utils/generic.h"
#include "cs_to_user.h"
#include "cs_to_bs.h"

BS_List *bs_list;
User *g_user;
Record_List *g_records;
char *g_data_dir = "cs_data";

int main(int argc, char **argv) {
  int c; /* Command option */
  int r_errno = 0;
  unsigned short int port; /* Port value for the servers */
  char *pvalue = NULL; /* Command port value */
  pid_t udp_server_pid; /* Child process ID */

  while ((c = getopt(argc, argv, "p:")) != -1) {
    switch (c) {
      case 'p':
        pvalue = optarg;
        if (pvalue != NULL) {
          port = parse_ushort_or_exit(pvalue, "port");
          break;
        }
      default:
        abort();
    }
  }

  // Use default port if none is provided.
  if (argc == 1) port = DEFAULT_PORT;

  // Make sure the CS data folder is present.
  mkdir(g_data_dir, 0770);

  // Prepare globals.
  bs_list = (BS_List *) malloc(sizeof(struct BS_List_t));
  bs_list->count = 0;
  bs_list->first = NULL;
  load_bs_list();
  g_records = (Record_List *)malloc(sizeof(struct backup_record_list_t));
  g_records->count = 0;
  g_records->first = NULL;

  // Start UDP server for BS registration.
  if ((udp_server_pid = fork()) == -1) {
    fprintf(stderr, "Unable to execute UDP server start.\n");
    return -1;
  }

  if (udp_server_pid == 0) {
    bs_handler(port);
  } /* if child process */
  else {
    r_errno = user_server(port);
  } /* parent process */

  if (r_errno != 0) display_error(r_errno);
  return 1;
}
