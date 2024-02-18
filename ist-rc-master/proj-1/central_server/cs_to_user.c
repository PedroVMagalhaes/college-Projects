#include "cs_to_user.h"
#include "cs_to_bs.h"

extern BS_List *bs_list;
extern User *g_user;
extern Record_List *g_records;
extern char *g_data_dir;
extern int errno;

int g_errno = 1;
int last_bs = 0;

int user_server(unsigned short int port) {
  int tcp_fd; /* TCP socket file descriptor */
  int user_fd; /* User client socket file descriptor */
  struct sockaddr_in addr; /* Socket address */
  socklen_t addrlen; /* Socket address size */
  pid_t user_pid;
  int ref;

  addr = config_addr("", port);
  tcp_fd = start_tcp_server(addr);

  addrlen = sizeof(addr);

  // TODO: add busy response, in case the the fork takes too long.
  while (g_errno == TRUE) {

    // Accept connections.
    do user_fd = accept(tcp_fd, (struct sockaddr*) &addr, &addrlen);
    while (user_fd == -1 && errno == EINTR);
    if (user_fd == -1) {
      close(user_fd);
      g_errno = EXIT_NO_USER_TCP_CONNECTION;
      continue;
    }

    // Fork to create child process to handle the connection concurrently.
    if ((user_pid = fork()) == -1) {
      g_errno = EXIT_USER_UNABLE_TO_FORK;
    }
    else if (user_pid == 0) {
      close(tcp_fd);
      printf("New Child\n");
      int i;
      for (i = 0; i < 2; i++) {
        printf("i: %d\n", i);
        handle_protocol_message(user_fd);
      }
      printf("End Child\n");
    }

    do ref = close(user_fd);
    while (ref == -1 && errno == EINTR);
    if (ref == -1) exit(-1);

  }

  close(tcp_fd);
  return g_errno;
}


void handle_protocol_message(int fd) {
  char *response;

  // Get code.
  char **tokens = get_n_tokens_from_file(TCP_SOCKET, fd, NULL, NULL, 1, DELIMITER);
  printf("%s\n", tokens[0]);

  // Call function to received 'code'.
  if (strcmp(tokens[0], AUT_CMD) == 0) {
    free_array(&tokens, 1);

    tokens = get_n_tokens_from_file(TCP_SOCKET, fd, NULL, NULL, 2, DELIMITER);
    printf("username: %s\npass: %s\n", tokens[0], tokens[1]);
    int result = verify_user(tokens[0], tokens[1]);
    printf("result: %d", result);
    if (result == WRONG_PASSWORD) {
      write(fd, (void *) "AUR NOK", 7);
    } else if (result == NO_USER) {
      if (create_user(tokens[0], tokens[1]) == -1) {
        write(fd, (void *) "AUR NOK", 7);
        return;
      }
      load_user(tokens[0], tokens[1]);
      write(fd, (void *) "AUR NEW", 7);

    } else {
      load_user(tokens[0], tokens[1]);
      write(fd, (void *) "AUR OK", 6);
    }
    free_array(&tokens, 2);

  // Delete current user.
  } else if (strcmp(tokens[0], DLU_CMD) == 0) {
    printf("Deleting: %s\n", g_user->filename);

    // Check if user has one directory backup (at least).
    if (g_records == NULL && unlink(g_user->filename) != -1) {
      write(fd, (void *) "DLR OK", 6);
    } else {
      write(fd, (void *) "DLR NOK", 7);
    }

    free_array(&tokens, 1);

  // Backup command.
  } else if (strcmp(tokens[0], BCK_CMD) == 0) {
    char *bs_info;
    int is_new_bs;
    free_array(&tokens, 1);

    tokens = get_n_tokens_from_file(TCP_SOCKET, fd, NULL, NULL, 2, DELIMITER);
    printf("token1: %s\n", tokens[0]);
    printf("token2: %s\n", tokens[1]);
    int n_files = atoi(tokens[1]);
    FileInfo ***files = (FileInfo ***) malloc(sizeof(FileInfo **));
    FileInfo ***user_files = (FileInfo ***) malloc(sizeof(FileInfo **));
    int error = get_n_file_info_from_file(TCP_SOCKET, fd, NULL, 0, DELIMITER, tokens[0], user_files, atoi(tokens[1]), FALSE);
    printf("Receive all files\n");

    // Load current user records and BS list.
    load_records();
    printf("load\n");
    load_bs_list();
    printf("bs\n");

    // Select a BS.
    if (dir_has_backup(tokens[0]) == TRUE) {
      printf("Has bck\n");
      bs_info = get_bs_info(tokens[0]);
      FileInfo ***bs_files = (FileInfo ***) malloc(sizeof(FileInfo **));
      int bs_files_len = bs_list_files(bs_info, tokens[0], bs_files);

      n_files = diff(files, user_files, atoi(tokens[1]), bs_files, bs_files_len);
      // TODO: free FileInfo** (bs user)

      is_new_bs = FALSE;
    } else {
      printf("No bck\n");
      bs_info = get_random_bs();
      is_new_bs = TRUE;
      files = user_files;
      printf("Generated random\n");
    }

    printf("Select BS\n");

    // Check if it's available.
    if (check_bs_availability(bs_info) == FALSE) {
      write(fd, (void *) "BKR EOF", 7);
    } else {
      printf("Availability checked\n");

      // First backup of dir.
      if (is_new_bs) {
        bs_create_user(bs_info);

        // Pass files to User.
        write(fd, "BKR ", 4);
        printf("Write something\n");
        if (error != NO_ERROR) {
          write(fd, "ERR", 3);
        } else {
          write(fd, bs_info, strlen(bs_info));
          write(fd, " ", 1);
          write(fd, (void *) tokens[1], strlen(tokens[1]));
          write(fd, (void *) " ", 1);
          print_files_and_data(TCP_SOCKET, fd, NULL, 0, *files, n_files, FALSE, " ", 1);
        }

      } else {
        write(fd, "BKR EOF", 7);
      }
    }

    free_array(&tokens, 2);

  // Restore command.
  } else if (strcmp(tokens[0], RST_CMD) == 0) {
    write(fd, "RST", 3);
    free_array(&tokens, 1);

  // List backed up directories command.
  } else if (strcmp(tokens[0], LSD_CMD) == 0) {

    // Load current user recors.
    load_records();

    response = list_dir();
    write(fd, "LDR ", 4);
    write(fd, response, strlen(response));
    free_array(&tokens, 1);

  // List backed up files command.
  } else if (strcmp(tokens[0], LSF_CMD) == 0) {
    char *bs_info;

    // Load current user recors.
    load_records();

    // Select a BS.
    if (dir_has_backup(tokens[1]) == TRUE) {
      printf("Has bck\n");
      bs_info = get_bs_info(tokens[1]);
    } else write(fd, "LFD NOK", 7);

    // Check if it's available.
    if (check_bs_availability(bs_info) == FALSE)
      write(fd, "LFD EOF", 7);
    else write(fd, "LFD [list of files]", 18);

    free_array(&tokens, 1);

  // Delete directory command.
  } else if (strcmp(tokens[0], DEL_CMD) == 0) {

    // Load current user recors.
    load_records();

    if (bs_delete_dir(get_bs_info(tokens[1]), tokens[1]) != -1)
      write(fd, "DDR OK", 6);
    else write(fd, "DDR NOK", 7);
    free_array(&tokens, 1);

  } else {
    printf("Error returned\n");
    write(fd, "ERR", 3);
    free_array(&tokens, 1);
  }

  write(fd, "\n", 1);

}


void clean_records() {
  Record *rec, *prev = NULL;

  // Delete records.
  for (rec = g_records->first; rec != NULL; prev = rec, rec = rec->next) {
    delete_record(prev);
  }

  // Last record.
  delete_record(prev);

  g_records->first = NULL;
}


void delete_record(Record *record) {
  int i;

  // Don't delete what does not exist.
  if (record == NULL) return;

  // Delete directories and information.
  for (i = 0; i < record->count; i++) free(record->dirs[i]);
  free(record->bs_info);
}


void load_records() {

  // Load nothing if there is no current user.
  if (g_user == NULL) return;

  FILE *user;
  char line[256];
  char **line_split;
  Record *record = NULL;

  // Check if there are any loaded records.
  if (g_records != NULL) clean_records();

  g_records = (Record_List *)malloc(sizeof(struct backup_record_list_t));
  g_records->first = NULL;

  if ((user = fopen(g_user->filename, "r")) == NULL) {
    fprintf(stderr, "Error opening user file for load.\n");
    exit(-1);
  }

  // Load backed up directories.
  while (fgets(line, sizeof(line), user) != NULL) {
    str_split(line, &line_split, "|");
    load_record(line_split[0], line_split[1], record);
    if (record == NULL) record = g_records->first;
    record = record->next;
    g_records->count++;
  }

  fclose(user);
}


void load_record(char *dir, char *bs_info, Record *prev) {
  Record *record;

  record = (Record *) malloc(sizeof(struct backup_record_t));
  record->count = str_split(dir, &record->dirs, " ");
  record->bs_info = bs_info;
  record->next = NULL;

  if (prev == NULL) g_records->first = record;
  else prev->next = record;
  prev = record;
}


char *list_dir() {
  int i;
  char *files;
  Record *record;

  for (record = g_records->first; record != NULL; record = record->next) {
    for (i = 0; i < record->count; i++) {
      files = str_join(files, record->dirs[i], " ");
    }
  }

  return files;
}


int dir_has_backup(char *dir) {
  Record *record;
  int i;

  printf("%d\n", g_records == NULL);
  printf("%d\n", g_records->first == NULL);

  printf("1\n");
  for (record = g_records->first; record != NULL; record = record->next) {
    printf("2\n");
    for (i = 0; i < record->count; i++) {
      printf("3\n");
      if (strcmp(record->dirs[i], dir) == 0) return 1;
    }
  }

  printf("4\n");
  return 0;
}


int check_bs_availability(char *id) {
  BS_Node *bs;
  for (bs = bs_list->first; bs != NULL; bs = bs->next)
    if (bs->id == id) return 1;
  return 0;
}


char *get_bs_info(char *dir) {
  Record *record;
  int i;

  for (record = g_records->first; record != NULL; record = record->next)
    for (i = 0; i < record->count; i++)
      if (record->dirs[i] == dir)
        return record->bs_info;

  return "";
}


char *get_random_bs() {
  BS_Node *bs;
  int i = 0;

  // No BS available.
  if (bs_list->first == NULL) {
    last_bs = 0;
    return "";
  }

  // Select the next server on the list.
  if (last_bs > bs_list->count) last_bs = 0;
  for (bs = bs_list->first; i < last_bs; i++, bs = bs->next);
  return bs->id;
}

int diff(FileInfo ***files, FileInfo ***u_files, int u_files_len, FileInfo ***b_files, int b_files_len) {
  int count = 0;

  for (int i = 0; i < u_files_len; i++) {
    for (int j = 0; j < b_files_len; j++) {
      if (
        strcmp((*u_files)[i]->_path, (*b_files)[j]->_path) == 0 &&
        difftime((*u_files)[i]->_date_time, (*b_files)[j]->_date_time) > 0
      ) {
        count++;
        *files = (FileInfo **) realloc((void *) *files, sizeof(FileInfo *) * count);
        break;
      } else if (j == b_files_len - 1) {
        count++;
        *files = (FileInfo **) realloc((void *) *files, sizeof(FileInfo *) * count);
      }
    }
  }

  return count;
}
