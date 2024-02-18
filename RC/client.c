#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define PORTGN "58055"

//client session info
typedef struct SESSION {
  char GID[128];
  char UID[128];
  char pass[128];
  char hostname[128];
  char port[128];
} Session;

void handle_registration_login(char *first_argument, char *second_argument, char *third_argument, Session *session);
void handle_user_session(char *first_argument, Session *session);
void handle_group_management(char *first_argument, char *second_argument, char *third_argument, Session *session);
void handle_group_listing(char *first_argument, Session *session);
void handle_posting(char *first_argument, char *second_argument, Session *session);
void handle_posting_file(char *first_argument, char *second_argument, char *third_argument, Session *session);
void handle_retrieving(char *second_argument, Session *session);
void udp_message(char *message, Session *session);
void tcp_message(char *message, Session *session);

void process_command(Session *session) {;
  char command[512] = "", first_argument[128] = "" , second_argument[512] = "", third_argument[128] = "";

  printf("Enter command: ");
  if (fgets(command, 512, stdin) == NULL) {
    printf("Error getting command\n");
    exit(1);
  }

  sscanf(command, "%s", first_argument);
  if (strcmp(first_argument, "post") == 0)
    sscanf(command, "%s \"%[^\"]\" %s", first_argument, second_argument, third_argument);
  else
    sscanf(command, "%s %s %s", first_argument, second_argument, third_argument);

  if (strcmp(first_argument, "reg") == 0 ||
      strcmp(first_argument, "unregister") == 0 ||
      strcmp(first_argument, "unr") == 0) {
    if (strlen(second_argument) != 5 || strlen(third_argument) != 8)
      printf("UID must have 5 digits and pass must have 8 characters\n");
    else
      handle_registration_login(first_argument, second_argument, third_argument, session);
  }

  else if (strcmp(first_argument, "login") == 0) {
    if (strlen(second_argument) != 5 || strlen(third_argument) != 8)
      printf("UID must have 5 digits and pass must have 8 characters\n");
    else {
      strcpy(session->UID, second_argument); strcpy(session->pass, third_argument);
      handle_registration_login(first_argument, second_argument, third_argument, session);
    }
  }

  else if (strcmp(first_argument, "logout") == 0 ||
           strcmp(first_argument, "showuid") == 0 ||
           strcmp(first_argument, "su") == 0 ||
           strcmp(first_argument, "exit") == 0 ||
           strcmp(first_argument, "groups") == 0 ||
           strcmp(first_argument, "gl") == 0 ) {
    if (strlen(second_argument) != 0 || strlen(third_argument) != 0)
      printf("Too many arguments\n");
    else
      handle_user_session(first_argument, session);
  }

  else if (strcmp(first_argument, "subscribe") == 0 ||
           strcmp(first_argument, "s") == 0) {
    if (strlen(second_argument) != 2 || strlen(third_argument) == 0 || strlen(third_argument) > 24)
      printf("GID must have 2 digits and GName must have 24 characters or less\n");
    else if (strlen(session->UID) == 0)
      printf("No user is logged in\n");
    else
      handle_group_management(first_argument, second_argument, third_argument, session);
  }
  else if (strcmp(first_argument, "unsubscribe") == 0 ||
           strcmp(first_argument, "u") == 0 ||
           strcmp(first_argument, "select") == 0 ||
           strcmp(first_argument, "sag") == 0) {
    if (strlen(second_argument) != 2)
      printf("GID must have 2 digits\n");
    else if (strlen(third_argument) != 0)
      printf("Too many arguments\n");
    else if (strlen(session->UID) == 0)
      printf("No user is logged in\n");
    else
      handle_group_management(first_argument, second_argument, third_argument, session);
  }

  else if (strcmp(first_argument, "my_groups") == 0 ||
           strcmp(first_argument, "mgl") == 0 ||
           strcmp(first_argument, "showgid") == 0 ||
           strcmp(first_argument, "sg") == 0) {
    if (strlen(second_argument) != 0 || strlen(third_argument) != 0)
      printf("Too many arguments\n");
    else if (strlen(session->UID) == 0)
      printf("No user is logged in\n");
    else
      handle_group_listing(first_argument, session);
  }
  else if (strcmp(first_argument, "ulist") == 0 ||
           strcmp(first_argument, "ul") == 0 ) {
    if (strlen(second_argument) != 0 || strlen(third_argument) != 0)
      printf("Too many arguments\n");
    else if (strlen(session->UID) == 0)
      printf("No user is logged in\n");
    else if (strlen(session->GID) == 0)
      printf("No group is selected\n");
    else
      handle_group_listing(first_argument, session);
  }

  else if (strcmp(first_argument, "post") == 0) {
    if (strlen(second_argument) == 0 || second_argument[strlen(second_argument)-1] == '\n')
      printf("Message must be between quotes\n");
    else if (strlen(second_argument) > 240)
      printf("Message must have 240 characters or less\n");
    else if (strlen(third_argument) > 24)
      printf("FName must have 24 characters or less\n" );
    else if (strlen(session->UID) == 0)
      printf("No user is logged in\n");
    else if (strlen(session->GID) == 0)
      printf("No group is selected\n");
    else if (strlen(third_argument) == 0)
      handle_posting(first_argument, second_argument, session);
    else
      handle_posting_file(first_argument, second_argument, third_argument, session);
  }

  else if (strcmp(first_argument, "retrieve") == 0 ||
           strcmp(first_argument, "r") == 0) {
    if (strlen(second_argument) != 4)
      printf("MID must have 4 digits\n");
    else if (strlen(session->UID) == 0)
      printf("No user is logged in\n");
    else if (strlen(session->GID) == 0)
      printf("No group is selected\n");
    else
      handle_retrieving(second_argument, session);
  }

  else
    printf("Invalid command\n");

  memset(command, 0, sizeof(command));
  memset(first_argument, 0, sizeof(first_argument));
  memset(second_argument, 0, sizeof(second_argument));
  memset(third_argument, 0, sizeof(third_argument));
}

void handle_registration_login(char *first_argument, char *second_argument, char *third_argument, Session *session) {
  char message[128];
  if (strcmp(first_argument, "reg") == 0)
    sprintf(message, "REG %s %s\n", second_argument, third_argument);
  else if (strcmp(first_argument, "unregister") == 0 || strcmp(first_argument, "unr") == 0)
    sprintf(message, "UNR %s %s\n", second_argument, third_argument);
  else if (strcmp(first_argument, "login") == 0)
    sprintf(message, "LOG %s %s\n", second_argument, third_argument);
  udp_message(message, session);
}

void handle_user_session(char *first_argument, Session *session) {
  char message[128];
  if (strcmp(first_argument, "logout") == 0) {
    sprintf(message, "OUT %s %s\n", session->UID, session->pass);
    memset(session->UID, 0, sizeof(session->UID)); memset(session->pass, 0, sizeof(session->pass));
    strcpy(session->UID, ""); strcpy(session->pass, "");
    udp_message(message, session);
  }
  else if (strcmp(first_argument, "showuid") == 0 || strcmp(first_argument, "su") == 0)
    printf("UID: %s\n", session->UID);
  else if (strcmp(first_argument, "exit") == 0) {
    if (strlen(session->UID) != 0 && strlen(session->pass) != 0) { //TODO ask if this is needed or just locally
      sprintf(message, "OUT %s %s\n", session->UID, session->pass);
      udp_message(message, session);
    }
    memset(session->UID, 0, sizeof(session->UID)); memset(session->pass, 0, sizeof(session->pass));
    strcpy(session->UID, ""); strcpy(session->pass, "");
    exit(1); //TODO ask about closing tcp connections and if logout is local
  }

  //would make more sense in handle_group_listing but works better here because of process_command
  else if (strcmp(first_argument, "groups") == 0 || strcmp(first_argument, "gl") == 0) {
    sprintf(message, "GLS\n");
    udp_message(message, session);
  }
}

void handle_group_management(char *first_argument, char *second_argument, char *third_argument, Session *session) {
  char message[128];
  if (strcmp(first_argument, "subscribe") == 0 || strcmp(first_argument, "s") == 0) {
    sprintf(message, "GSR %s %s %s\n", session->UID, second_argument, third_argument);
    udp_message(message, session);
  }
  else if (strcmp(first_argument, "unsubscribe") == 0 || strcmp(first_argument, "u") == 0) {
    sprintf(message, "GUR %s %s\n", session->UID, second_argument);
    udp_message(message, session);
  }
  else if (strcmp(first_argument, "select") == 0 || strcmp(first_argument, "sag") == 0)
    strcpy(session->GID, second_argument);
}

void handle_group_listing(char *first_argument, Session *session) {
  char message[128];
  if (strcmp(first_argument, "my_groups") == 0 || strcmp(first_argument, "mgl") == 0) {
    sprintf(message, "GLM %s\n", session->UID);
    udp_message(message, session);
  }
  else if (strcmp(first_argument, "showgid") == 0 || strcmp(first_argument, "sg") == 0)
    printf("GID: %s\n", session->GID);
  else if (strcmp(first_argument, "ulist") == 0 || strcmp(first_argument, "ul") == 0) {
    sprintf(message, "ULS %s\n", session->GID);
    tcp_message(message, session);
  }
}

void handle_posting(char *first_argument, char *second_argument, Session *session) {
  char message[512];
  sprintf(message, "PST %s %s %d %s\n", session->UID, session->GID, strlen(second_argument), second_argument);
  tcp_message(message, session);
}

void handle_posting_file(char *first_argument, char *second_argument, char *third_argument, Session *session) {
  int fd, errcode;
  ssize_t n, size, bytes_read;
  socklen_t addrlen;
  struct addrinfo hints, *res;
  struct sockaddr_in addr;
  char message[512], buffer[2048], *dot;
  FILE *fp;

  dot = strrchr(third_argument, '.');
  if (dot == NULL) {
    printf("File not found\n");
    return;
  }
  if (strcmp(dot+1, "txt") == 0) {
    fp = fopen(third_argument, "r");
    if (fp == NULL) {
      printf("File not found\n");
      return;
    }
  }
  else {
    fp = fopen(third_argument, "rb");
    if (fp == NULL) {
      printf("File not found\n");
      return;
    }
  }

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    exit(1);
  }
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  errcode = getaddrinfo(session->hostname, session->port, &hints, &res);
  if (errcode != 0) {
    exit(1);
  }

  n = connect(fd, res->ai_addr, res->ai_addrlen);
  if (n == -1) {
    exit(1);
  }

  sprintf(message, "PST %s %s %d %s %s %d ", session->UID, session->GID, strlen(second_argument), second_argument, third_argument, size);
  n = write(fd, message, strlen(message));
  if (n == -1) {
    exit(1);
  }
  while (bytes_read = fread(buffer, 1, sizeof(buffer), fp)) {
    n = write(fd, buffer, bytes_read);
    if (n == -1) {
      exit(1);
    }
  }
  fclose(fp);
  while(bytes_read = read(fd, buffer, sizeof(buffer))) {
    write(1, buffer, bytes_read);
  }

  freeaddrinfo(res);
  close(fd);
}

void handle_retrieving(char *second_argument, Session *session) {
  int fd, errcode, i = 0;
  ssize_t n, bytes_read;
  socklen_t addrlen;
  struct addrinfo hints, *res;
  struct sockaddr_in addr;
  char buffer[2048], message[512];
  char status[128], N[128], msg_id[128], uid[128], msg_size[128], msg[128], filename[128], file_size[128];
  FILE *fp;

  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    exit(1);
  }
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  errcode = getaddrinfo(session->hostname, session->port, &hints, &res);
  if (errcode != 0) {
    exit(1);
  }

  n = connect(fd, res->ai_addr, res->ai_addrlen);
  if (n == -1) {
    exit(1);
  }

  sprintf(message, "RTV %s %s %s\n", session->UID, session->GID, second_argument);
  n = write(fd, message, strlen(message));
  if (n == -1) {
    exit(1);
  }

  fp = fopen("file.txt", "w");
  while(bytes_read = read(fd, buffer, sizeof(buffer))) {
    i = sscanf(buffer, "%*[^/]/ %s", filename);
    fwrite(filename, 1, bytes_read, fp);
    /*if (i != 0 && i != -1)
      printf("%s %s %s %s %s %s\n", msg_id, uid, msg_size, msg, filename, file_size);*/
    /*if (i == 8) {
      fp = fopen(filename, "wb");
      fwrite(buffer, 1, bytes_read, fp);
    }
    else
      fwrite(1, buffer, bytes_read);*/
  }
  fclose(fp);
  freeaddrinfo(res);
  close(fd);
}

//TODO add timeout to udp and tcp
void udp_message(char *message, Session *session) {
  int fd, errcode;
  ssize_t n;
  socklen_t addrlen;
  struct addrinfo hints, *res;
  struct sockaddr_in addr;
  char buffer[2048]; //larger array size to handle 'groups' command

  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == -1) {
    exit(1);
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  errcode = getaddrinfo(session->hostname, session->port, &hints, &res);
  if (errcode != 0) {
    exit(1);
  }
  n = sendto(fd, message, strlen(message), 0, res->ai_addr, res->ai_addrlen);
  if (n == -1) {
    exit(1);
  }
  addrlen = sizeof(addr);
  n = recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr*) &addr, &addrlen);
  if (n == -1) {
    exit(1);
  }

  write(1, buffer, n);

  freeaddrinfo(res);
  close(fd);
}

void tcp_message(char *message, Session *session) {
  int fd, errcode;
  ssize_t n, bytes_read;
  socklen_t addrlen;
  struct addrinfo hints, *res;
  struct sockaddr_in addr;
  char buffer[2048];

  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    exit(1);
  }
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  errcode = getaddrinfo(session->hostname, session->port, &hints, &res);
  if (errcode != 0) {
    exit(1);
  }

  n = connect(fd, res->ai_addr, res->ai_addrlen);
  if (n == -1) {
    exit(1);
  }

  n = write(fd, message, strlen(message));
  if (n == -1) {
    exit(1);
  }
  while(bytes_read = read(fd, buffer, sizeof(buffer))) {
    write(1, buffer, bytes_read);
  }

  freeaddrinfo(res);
  close(fd);
}

void parse_input(int argc, char **argv, Session *session) {
  if (argc == 1) {
    strcpy(session->hostname, "localhost");
    strcpy(session->port, PORTGN);
  }

  //TODO remove after testing
  else if (strcmp(argv[1], "1") == 0) {
    strcpy(session->hostname, "tejo.tecnico.ulisboa.pt");
    strcpy(session->port, "58011");
  }

  else if (argc == 3) {
    if (strcmp(argv[1], "-n") != 0) {
      printf("Wrong flags\n");
      exit(1);
    }
    else {
      strcpy(session->hostname, argv[2]);
      strcpy(session->port, PORTGN);

    }
  }
  else if (argc == 5) {
    if (strcmp(argv[1], "-n") != 0 || strcmp(argv[3], "-p") != 0) {
      printf("Wrong flags\n");
      exit(1);
    }
    else {
      strcpy(session->hostname, argv[2]);
      strcpy(session->port, argv[4]);
    }
  }
  else {
    printf("Wrong number of arguments\n");
    exit(1);
  }
}

int main(int argc, char **argv) {
  Session session;
  strcpy(session.GID, "");
  strcpy(session.UID, "");
  strcpy(session.pass, "");
  strcpy(session.hostname, "");
  strcpy(session.port, "");
  parse_input(argc, argv, &session);
  while(1)
    process_command(&session);
  return 0;
}
