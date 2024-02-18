#include "generic.h"


unsigned short int parse_ushort_or_exit(char const *str, char const *name) {
  unsigned short int value; /* Parsed value */

  if (sscanf(str, "%hu", &value) != 1) {
    fprintf(stderr, "\nError on argument \"%s\".\n\n", name);
    exit(-1);
  }
  if (value < 1) {
    fprintf(stderr, "\nValue of \"%s\" should be bigger than", name);
    exit(-1);
  }
  return value;
}


int get_number_of_digits(int number) {
  int res = 0;
  while ((number /= 10) > 0) {
    res++;
  }

  return res++;
}


char *strdup(const char *str) {
  int str_len = strlen(str) + 1;
  char *new_str = (char *) calloc(str_len, sizeof(char));

  if (new_str == NULL) {
    fprintf(stderr, "Cannot allocate memory\n");
    exit(-1);
  }

  memcpy(new_str, str, str_len);
  return new_str;
}


char *nstrdup(char **dest, int *dest_len, int n_str, ...) {
  va_list str_list, str_list_2; /* We can go througth each list only one time */
  *dest_len = 0;

  va_start(str_list, n_str);
  va_copy(str_list_2, str_list);

  for (int i = 0; i < n_str; i++) {
    const char *str = va_arg(str_list, char *);

    if(str != NULL) {
      *dest_len += strlen(str);
    }
  }
  va_end(str_list);

  char *aux = (char *) calloc(*dest_len + 1, sizeof(char));

  for (int i = 0; i < n_str; i++) {
    const char *str = va_arg(str_list_2, char *);

    if(str != NULL) {
      strcat(aux, str);
    }
  }
  va_end(str_list_2);

  return (*dest = aux);
}

int strtok_count(char *str, const char *delim) {
  int count = 0;
  char *token = strtok(str, delim);

  while (token != NULL) {
    count++;
    token = strtok(NULL, delim);
  }

  free(str);
  return count;
}

int str_split(char *str, char ***split_array, const char *delim) {
  int size;
  char *token;
  int i = 0;

  size = strtok_count(strdup(str), delim);
  *split_array = (char **) calloc(size, sizeof(char *));

  token = strtok(str, delim);

  /* NULL uses the last remaining string. */
  while (token != NULL) {
    (*split_array)[i] = strdup(token);
    token = strtok(NULL, delim);
    i++;
  }

  return size;
}


char *str_join(char *str1, char *str2, const char *glue) {
  int size = strlen(str1) + strlen(str2) + strlen(glue) + 1;
  char *new_str = (char *)malloc(size * sizeof(char));

  if (sprintf(new_str, "%s%s%s", str1, glue, str2) == -1) exit(-1);

  return new_str;
}


char *array_join(char ***arr, int arr_size, const char *glue) {
  int i;
  char *str = "";
  for (i = 0; i < arr_size; i++)
    str = str_join(str, (*arr)[i], glue);
  return str;
}


int check_string_len_and_chars(
  const char *str, const unsigned int str_len,
  const char ascii_cont[][2],
  const unsigned int ascii_cont_len
) {

  for (int i = 0; i < str_len; i++) {
    for (int j = 0; j < 2; j++) {
      if (str[i] >= ascii_cont[i][j] && str[i] <= ascii_cont[i][j]) {
        break; /* go to the next character */
      }
    }

    /* if condition failed there is a character that is not in
    any ascii container */
    return STRING_HAS_INVALID_CHARACTERS;
  }

  return 1;
}


void free_array(char ***arr, int size) {
  int i;
  for (i = 0; i < size; i++)
    free((*arr)[i]);

  free((*arr));
}


void display_error(int err_code) {
  switch (err_code) {
    case EXIT_NO_USER_TCP_CONNECTION:
      fprintf(stderr, "Error accpeting user TCP connection.\n");
      break;
    case EXIT_USER_UNABLE_TO_FORK:
      fprintf(stderr, "Unable to fork process to handle new TCP connection.\n");
      break;
    default:
      fprintf(stderr, "An error occurred.\n");
  }
}


int check_string_len(const char *str, const unsigned int max_str_len) {
  if (strlen(str) > max_str_len) {
    return STRING_LENGTH_IS_GREATER_THAN_MAX_STR_LEN;
  }

  return NO_ERROR;
}


int check_string_chars(
  const char *str,
  const unsigned int str_len,
  const char ascii_cont[][2],
  const unsigned int ascii_cont_len
) {

  int valid_char = FALSE;
  for (int i = 0; i < str_len; i++) {
    for (int j = 0; j < ascii_cont_len; j++) {
      if (!valid_char) {
        if (str[i] >= ascii_cont[j][0] && str[i] <= ascii_cont[j][1]) {
          valid_char = TRUE;
        }
      }
    }

    if (!valid_char) {
      return STRING_HAS_INVALID_CHARACTERS;
    }

    valid_char = FALSE;
  }

  return NO_ERROR;
}


int is_str_in_container(
  const char **container,
  const int container_len,
  const char *str
) {

  int in = -1; /* == S_STATUS.S_FALSE */
  for (int i = 0; i < container_len; i++) {
    if (strcmp(str, container[i]) == 0) {
      in = i; /* Parse to S_STATUS */
      break;
    }
  }

  return in;
}


const char *get_default_error_message(const int error_number) {
  switch (error_number) {
    case STRING_LENGTH_IS_GREATER_THAN_MAX_STR_LEN:
      return "Has a length greater than what is expected";

    case STRING_HAS_INVALID_CHARACTERS:
      return "Has some invalid characters";

    case CANNOT_PARSE_STRING_TO_INT:
      return "Error when parsing CSport to an intenger";

    case INVALID_NUMBER_OF_ARGS:
      return "Number of arguments can only be 1, 3 or 5";

    case INVALID_OPT_ARG_SYNTAX:
      return "Argument should be an option (eg: '-n')";

    case DUPLICATE_OPT_ARG:
      return "Referring the same option";
  }

  return "";
}


struct sockaddr_in *get_server_addr(
  const int domain,
  const u_int16_t server_port,
  struct hostent *server_ent
) {

  struct sockaddr_in *server_addr;

  server_addr = (struct sockaddr_in*) calloc(1, sizeof(struct sockaddr_in));
  server_addr -> sin_family = domain;

  /* Convert IPv4 address from char* to usint32_t */
  server_addr -> sin_addr.s_addr = ((struct in_addr *)
                                    (server_ent -> h_addr_list[0])) -> s_addr;

  /* parse server_port to network byte order */
  server_addr -> sin_port = htons(server_port);
  return server_addr;
}


int connect_to_server_by_name(
  const int c_protocol,
  const int domain,
  const char *server_name,
  const uint16_t server_port
) {

  struct sockaddr_in *server_addr = get_server_addr(domain, server_port, gethostbyname(server_name));
  return connect_to_server(c_protocol, domain, server_addr);
}


int connect_to_server(
  const int c_protocol,
  const int domain,
  const struct sockaddr_in *server_addr
) {

  int fd = socket(domain, c_protocol, 0);
  if (connect(fd, (struct sockaddr*) server_addr,
              (socklen_t) sizeof(*server_addr)) == -1) {

    fprintf(stderr, "Error while connecting to server\n");
    exit(-1);
  }

  return fd;
}
