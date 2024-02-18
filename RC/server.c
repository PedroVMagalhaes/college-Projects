#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#define PORT "58055"

typedef struct INFO {
  int gid;
  char curr_gid[10];
  char grp_no[99][10];
  char grp_name[99][128];
  char msg_ids[99][10];
} Info;

int validate_UID(char *UID);
int validate_GID(char *GID);
char *handle_register(char *UID, char *pass);
char *handle_unregister(char *UID, char *pass);
char *handle_login(char *UID, char *pass);
char *handle_logout(char *UID, char *pass);
void list_groups_dir(Info *info);
void handle_group_list(char *reply, Info *info);
void handle_group_subscription(char *UID, char *GID, char *GName, char *status, Info *info);
void handle_group_unsubscription(char *UID, char *GID, char *status);
void handle_user_group_list(char *UID, char *reply, Info *info);

void process_command(char *command, char *message, Info *info) {
  char first_argument[128] = "", second_argument[128] = "", third_argument[128] = "", fourth_argument[128] = "";
  char reply[512] = "";

  sscanf(command, "%s %s %s %s", first_argument, second_argument, third_argument, fourth_argument);

  if (strcmp(first_argument, "REG") == 0) {
    strcpy(reply, handle_register(second_argument, third_argument));
    sprintf(message, "RRG %s\n", reply);
  }
  else if (strcmp(first_argument, "UNR") == 0) {
    strcpy(reply, handle_unregister(second_argument, third_argument));
    sprintf(message, "RUN %s\n", reply);
  }
  else if (strcmp(first_argument, "LOG") == 0) {
    strcpy(reply, handle_login(second_argument, third_argument));
    sprintf(message, "RLO %s\n", reply);
  }
  else if (strcmp(first_argument, "OUT") == 0) {
    strcpy(reply, handle_logout(second_argument, third_argument));
    sprintf(message, "ROU %s\n", reply);
  }
  else if (strcmp(first_argument, "GLS") == 0) {
    handle_group_list(reply, info);
    sprintf(message, "RGL %s\n", reply);
  }
  else if (strcmp(first_argument, "GSR") == 0) {
    handle_group_subscription(second_argument, third_argument, fourth_argument, reply, info);
    sprintf(message, "RGS %s\n", reply);
  }
  else if (strcmp(first_argument, "GUR") == 0) {
    handle_group_unsubscription(second_argument, third_argument, reply);
    sprintf(message, "RGU %s\n", reply);
  }
  else if (strcmp(first_argument, "GLM") == 0) {
    handle_user_group_list(second_argument, reply, info);
    sprintf(message, "RGM %s\n", reply);
  }
  /*else if (strcmp(first_argument, "ULS"))
  else if (strcmp(first_argument, "PST"))
  else if (strcmp(first_argument, "RTV"))*/
  else
    strcpy(message, "ERR\n");

  memset(command, 0, sizeof(command));
  memset(first_argument, 0, sizeof(first_argument));
  memset(second_argument, 0, sizeof(second_argument));
  memset(third_argument, 0, sizeof(third_argument));
  memset(fourth_argument, 0, sizeof(fourth_argument));
}

int validate_UID(char *UID) {
  int i = 0;
  if (strlen(UID) != 5)
    return 1;
  for (i = 0; i < strlen(UID); i++) {
    if (!isdigit(UID[i]))
      return 1;
  }
  return 0;
}

int validate_GID(char *GID) {
  int i = 0;
  if (strlen(GID) != 2)
    return 1;
  for (i = 0; i < strlen(GID); i++) {
    if (!isdigit(GID[i]))
      return 1;
  }
  return 0;
}

char *handle_register(char *UID, char *pass) {
  int n;
  char dir[128], file[128];
  FILE *fp;

  if (validate_UID(UID) == 1 || strlen(pass) != 8)
    return "NOK";

  sprintf(dir, "USERS/%s", UID);
  n = mkdir(dir, 0700);
  if (n == -1) {
    if (errno == EEXIST)
      return "DUP";
    else
      return "NOK";
  }
  sprintf(file, "%s/%s_pass.txt", dir, UID);
  fp = fopen(file, "w");
  fputs(pass, fp);
  fclose(fp);
  return "OK";
}

char *handle_unregister(char *UID, char *pass) {
  int n;
  char dir[128], path_pass[128], path_login[128], buffer[128];
  FILE *fp;

  sprintf(dir, "USERS/%s", UID);
  sprintf(path_pass, "%s/%s_pass.txt", dir, UID);
  sprintf(path_login, "%s/%s_login.txt", dir, UID);
  if (validate_UID(UID) == 1 || access(dir, F_OK) != 0 || access(path_pass, F_OK) != 0 || strlen(pass) != 8)
    return "NOK";
  fp = fopen(path_pass, "r");
  fgets(buffer, 128, fp);
  if (strcmp(buffer, pass) != 0) {
    fclose(fp);
    return "NOK";
  }
  fclose(fp);
  unlink(path_pass);
  if (access(path_login, F_OK) == 0)
    unlink(path_login);
  while(1) {
    n = rmdir(dir);
    if (n == 0)
      break;
  }
  return "OK";
}

char *handle_login(char *UID, char *pass) {
  char dir[128], path_pass[128], path_login[128], buffer[128];
  FILE *fp;

  sprintf(dir, "USERS/%s", UID);
  sprintf(path_pass, "%s/%s_pass.txt", dir, UID);
  sprintf(path_login, "%s/%s_login.txt", dir, UID);
  if (validate_UID(UID) == 1 || access(dir, F_OK) != 0 || access(path_pass, F_OK) != 0 || strlen(pass) != 8)
    return "NOK";
  fp = fopen(path_pass, "r");
  fgets(buffer, 128, fp);
  if (strcmp(buffer, pass) != 0) {
    fclose(fp);
    return "NOK";
  }
  fclose(fp);
  fp = fopen(path_login, "w");
  fclose(fp);
  return "OK";
}

char *handle_logout(char *UID, char *pass) {
  char dir[128], path_pass[128], path_login[128], buffer[128];
  FILE *fp;

  sprintf(dir, "USERS/%s", UID);
  sprintf(path_pass, "%s/%s_pass.txt", dir, UID);
  sprintf(path_login, "%s/%s_login.txt", dir, UID);
  if (validate_UID(UID) == 1 || access(dir, F_OK) != 0 || access(path_pass, F_OK) != 0)
    return "NOK";
  fp = fopen(path_pass, "r");
  fgets(buffer, 128, fp);
  if (strcmp(buffer, pass) != 0) {
    fclose(fp);
    return "NOK";
  }
  fclose(fp);
  if (access(path_login, F_OK) == 0)
    unlink(path_login);
  return "OK";
}

void list_groups_dir(Info *info) {
  DIR *d, *m;
  struct dirent *dir, *msgs;
  int i = 0;

  FILE *fp;
  char GIDName[30], msgs_path[30];

  d = opendir("GROUPS");

  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (dir->d_name[0]=='.')
        continue;
      if (strlen(dir->d_name) > 2)
        continue;
      strcpy(info->grp_no[i], dir->d_name);
      strcpy(info->msg_ids[i], "0000");
      sprintf(msgs_path, "GROUPS/%s/MSG", dir->d_name);
      m = opendir(msgs_path);
      if (m) {
        while ((msgs = readdir(m)) != NULL) {
          if (msgs->d_name[0]=='.')
            continue;
          if (strlen(msgs->d_name) > 4)
            continue;
          strcpy(info->msg_ids[i], msgs->d_name);
        }
      }
      sprintf(GIDName, "GROUPS/%s/%s_name.txt", dir->d_name, dir->d_name);
      fp = fopen(GIDName, "r");
      if (fp) {
        fscanf(fp, "%24s", info->grp_name[i]);
        fclose(fp);
      }
      ++i;
      if (i == 99)
        break;
    }
    info->gid = i;
    i++;
    if (i < 10)
      sprintf(info->curr_gid, "0%d", i);
    else
      sprintf(info->curr_gid, "%d", i);
    closedir(d);
  }
}

void handle_group_list(char *reply, Info *info) {
  int i = 0;

  list_groups_dir(info);
  if (info->gid == 0) {
    strcpy(reply, "0");
    return;
  }
  else {
    sprintf(reply, "%d", info->gid);
    for (i = 0; i < info->gid; i++) {
      sprintf(reply, "%s %s %s %s", reply, info->grp_no[i], info->grp_name[i], info->msg_ids[i]);
    }
  }
}

void handle_group_subscription(char *UID, char *GID, char *GName, char *status, Info *info) {
  int n = 0, i = 0;
  char dir_gid[128], dir_uid[128], msg[128], path_name[128], path_uid[128], path_login[128];
  char buffer[128];
  FILE *fp;

  if (validate_GID(GID) == 1) {
    strcpy(status, "E_GRP");
    return;
  }
  if(strlen(GName) == 0 || strlen(GName) > 24) {
    strcpy(status, "E_GNAME");
    return;
  }
  sprintf(dir_uid, "USERS/%s", UID);
  sprintf(path_login, "%s/%s_login.txt", dir_uid, UID);
  if (validate_UID(UID) == 1 || access(dir_uid, F_OK) != 0 || access(path_login, F_OK) != 0) {
    strcpy(status, "E_USR");
    return;
  }

  sprintf(dir_gid, "GROUPS/%s", GID);
  if (strcmp(GID, "00") == 0) {
    list_groups_dir(info);
    if (info->gid == 99) {
      strcpy(status, "E_FULL");
      return;
    }
    sprintf(dir_gid, "GROUPS/%s", info->curr_gid);
    sprintf(path_name, "%s/%s_name.txt", dir_gid, info->curr_gid);
    sprintf(path_uid, "%s/%s.txt", dir_gid, UID);
    for (i = 0; i < info->gid; i++) {
      if (strcmp(info->grp_name[i], GName) == 0) {
        strcpy(status, "E_GRP");
        return;
      }
    }
    n = mkdir(dir_gid, 0700);
    if (n == -1)
        strcpy(status, "NOK");
    fp = fopen(path_name, "w");
    fputs(GName, fp);
    fclose(fp);
    sprintf(msg, "%s/MSG", dir_gid);
    mkdir(msg, 0700);
    fp = fopen(path_uid, "w");
    fclose(fp);
    sprintf(status, "NEW %s", info->curr_gid);
    return;
  }

  else {
    if (access(dir_gid, F_OK) != 0) {
      strcpy(status, "E_GRP");
      return;
    }
    sprintf(path_name, "%s/%s_name.txt", dir_gid, GID);
    if (access(path_name, F_OK) != 0) {
      strcpy(status, "NOK");
      return;
    }
    fp = fopen(path_name, "r");
    fgets(buffer, 128, fp);
    if (strcmp(buffer, GName) != 0) {
      fclose(fp);
      strcpy(status, "E_GNAME");
      return;
    }
    fclose(fp);
    sprintf(path_uid, "%s/%s.txt", dir_gid, UID);
    fp = fopen(path_uid, "w");
    fclose(fp);
    strcpy(status, "OK");
  }
}

void handle_group_unsubscription(char *UID, char *GID, char *status) {
  char dir_gid[128], dir_uid[128], path_uid[128], path_login[128];

  if (validate_GID(GID) == 1) {
    strcpy(status, "E_GRP");
    return;
  }
  sprintf(dir_uid, "USERS/%s", UID);
  sprintf(path_login, "%s/%s_login.txt", dir_uid, UID);
  if (validate_UID(UID) == 1 || access(dir_uid, F_OK) != 0 || access(path_login, F_OK) != 0) {
    strcpy(status, "E_USR");
    return;
  }

  sprintf(dir_gid, "GROUPS/%s", GID);
  if (access(dir_gid, F_OK) != 0) {
    strcpy(status, "E_GRP");
    return;
  }
  sprintf(path_uid, "%s/%s.txt", dir_gid, UID);
  if (access(path_uid, F_OK) == 0)
    unlink(path_uid);
  strcpy(status, "OK");
}

void handle_user_group_list(char *UID, char *reply, Info *info) {
  int i = 0, j = 0, g = 0;
  int gids[99];
  char dir_uid[128], path_login[128], path_uid[128];
  DIR *d;
  struct dirent *dir;

  sprintf(dir_uid, "USERS/%s", UID);
  sprintf(path_login, "%s/%s_login.txt", dir_uid, UID);
  if (validate_UID(UID) == 1 || access(dir_uid, F_OK) != 0 || access(path_login, F_OK) != 0) {
    strcpy(reply, "E_USR");
    return;
  }

  list_groups_dir(info);
  d = opendir("GROUPS");
  if (d) {
    for (j = 0; j < 99; j++)
      gids[j] = -1;
    while ((dir = readdir(d)) != NULL) {
      if (dir->d_name[0]=='.')
        continue;
      if (strlen(dir->d_name) > 2)
        continue;
      sprintf(path_uid, "GROUPS/%s/%s.txt", dir->d_name, UID);
      if (access(path_uid, F_OK) == 0) {
        gids[g] = i;
        g++;
      }
      ++i;
      if (i == 99)
        break;
    }
    sprintf(reply, "%d%", g);
    if (g == 0)
      return;
    for (j = 0; j < g; j++) {
      if (gids[j] == -1)
        break;
      sprintf(reply, "%s %s %s %s", reply, info->grp_no[gids[j]], info->grp_name[gids[j]], info->msg_ids[gids[j]]);
    }
  }
}


void message_protocol(char *message, Info *info) {
  int fd, errcode;
  ssize_t n;
  socklen_t addrlen;
  struct addrinfo hints, *res;
  struct sockaddr_in addr;
  char buffer[128];

  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == -1) {
    exit(1);
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  errcode = getaddrinfo(NULL, PORT, &hints, &res);
  if (errcode != 0) {
    exit(1);
  }

  n = bind(fd, res->ai_addr, res->ai_addrlen);
  if (n == -1) {
    exit(1);
  }

  while (1) {
    addrlen = sizeof(addr);
    n = recvfrom(fd, buffer, 128, 0, (struct sockaddr*) &addr, &addrlen);
    if (n == -1) {
        exit(1);
    }

    write(1, "Client command: ", 16);
    write(1, buffer, n);

    memset(message, 0, sizeof(message));
    process_command(buffer, message, info);
    write(1, message, strlen(message));
    n = sendto(fd, message, strlen(message), 0, (struct sockaddr*) &addr, addrlen);
    if (n == -1) {
      exit(1);
    }
  }

  freeaddrinfo(res);
  close(fd);
}

int main() {
  char message[2048];
  Info info;
  message_protocol(message, &info);
  return 0;
}
