#include "auth.h"

extern User *g_user;
extern char *g_data_dir;

int verify_user(char *username, char *password) {
  DIR *dir;
  struct dirent *ent;
  char **user;
  int size;

  if ((dir = opendir(g_data_dir)) == NULL) {
    fprintf(stderr, "Could not open CS record directory.\n");
    return -1;
  }


  while ((ent = readdir(dir)) != NULL) {
    if (ent->d_type != DT_REG) continue;

    char *f_name = strdup(ent->d_name);

    size = str_split(strtok(f_name, "."), &user, "_");
    if (size != 2) continue;
    // Check username.
    if (strcmp(username, user[0]) == 0) {
      // Check password.
      if (strcmp(password, user[1]) == 0) return 1;
      else return WRONG_PASSWORD;
    }

    free(f_name);
    free_array(&user, size);
  }

  closedir(dir);
  return NO_USER;
}


int create_user(char *username, char *password) {
  FILE *new_user;
  char *filename = (char *) malloc(FILENAME_SIZE * sizeof(char));

  // Create user persistent filename reference.
  if (sprintf(filename, "%s/%s_%s.txt", g_data_dir, username, password) == -1) {
    fprintf(stderr, "Error building filename (create_user).\n");
    free(filename);
    return -1;
  }


  // Create file for user registration.
  if ((new_user = fopen(filename, "w")) == NULL) {
    fprintf(stderr, "Could not create file on CS folder.\n");
    free(filename);
    return -1;
  }

  free(filename);
  fclose(new_user);
  return TRUE;
}


int load_user(char *username, char *password) {
  char *filename = (char *) malloc(FILENAME_SIZE * sizeof(char));

  if (sprintf(filename, "%s/%s_%s.txt", g_data_dir, username, password) == -1) {
    fprintf(stderr, "Error building filename (verify_user).\n");
    return -1;
  }

  if (g_user != NULL) clean_user();

  g_user = (User *) malloc(sizeof(struct user_t));
  g_user->username = strdup(username);
  g_user->password = strdup(password);
  g_user->filename = strdup(filename);

  free(filename);
  return TRUE;
}


void clean_user() {
  free(g_user->filename);
  free(g_user->username);
  free(g_user->password);
  free(g_user);
  g_user = NULL;
}
