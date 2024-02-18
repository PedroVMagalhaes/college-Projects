#include "user.h"

User *g_user;

/* "./user"
   "./user -n CS_NAME"
   "./user -p CS_PORT"
   "./user -n CS_NAME -p CS_PORT" */
int main(int argc, char* argv[]) {
  const char *options = "n:p:";
  int current_opt_char;

  char *cs_name = NULL;
  u_int16_t *cs_port = (u_int16_t *) malloc(sizeof(u_int16_t));

  /* TODO: Check if the same option appears more than one time */
  while ((current_opt_char = getopt(argc, argv, options)) != -1) {
    switch (current_opt_char) {
      case 'n': /* CS name */
        cs_name = strdup(optarg);
        break;

      case 'p':
        *cs_port = (u_int16_t) atoi(optarg);
        break;

      case '?':
        exit(-1);
        break;
    }
  }

  wait_for_command(cs_name, cs_port);

  free(cs_name);
  free(cs_port);
  free_User(g_user);
  return 0;
}

