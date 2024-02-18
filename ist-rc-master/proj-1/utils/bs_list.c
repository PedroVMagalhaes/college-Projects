#include "bs_list.h"

extern BS_List *bs_list;
extern char *g_data_dir;

void load_bs_list() {
  char *filename = (char *) malloc(FILENAME_SIZE * sizeof(char));
  char line[256];
  FILE *bs_list_fd;
  BS_Node *bs, *prev;

  clean_bs_list();

  if (sprintf(filename, "%s/bs_list.txt", g_data_dir) == -1) {
    fprintf(stderr, "Error building backup server log filename.\n");
    exit(-1);
  }

  struct stat st;
  if (stat(filename, &st) == -1) return;

  if ((bs_list_fd = fopen(filename, "r")) == NULL) return;

  while (fgets(line, sizeof(line), bs_list_fd) != NULL) {
    line[strlen(line) - 1] = '\0';
    bs = (BS_Node *)malloc(sizeof(struct bs_node_t));
    bs->id = strdup(line);
    bs->next = NULL;
    if (bs_list->first == NULL) bs_list->first = bs;
    else prev->next = bs;
    prev = bs;
    bs = NULL;
    bs_list->count++;
  }

  fclose(bs_list_fd);
}


void clean_bs_list() {
  BS_Node *node, *prev = NULL;

  if (bs_list->first != NULL) {
    node = bs_list->first;
    while (node != NULL) {
      prev = node;
      node = node->next;
      free(prev->id);
      free(prev);
    }
  }

  bs_list->first = NULL;
  bs_list->count = 0;
}


int add_bs(char *ip_port) {
  char *filename = (char *) malloc(FILENAME_SIZE * sizeof(char));
  BS_Node *new_bs, *node, *prev = NULL;
  FILE *bs_list_fd;

  // Don't duplicate BS
  for (node = bs_list->first; node != NULL; prev = node, node = node->next)
    if (strcmp(node->id, ip_port) == 0) return 0;

  // Build new structure.
  new_bs = (BS_Node *) malloc(sizeof(struct bs_node_t));
  new_bs->id = strdup(ip_port);
  new_bs->next = NULL;

  // Write id to file.
  if (sprintf(filename, "%s/bs_list.txt", g_data_dir) == -1) {
    fprintf(stderr, "Error building backup server log filename.\n");
    exit(-1);
  }

  if ((bs_list_fd = fopen(filename, "a")) == NULL) return 0;
  fprintf(bs_list_fd, "%s", new_bs->id);
  fclose(bs_list_fd);

  // Add to list in memory.
  if (bs_list->first == NULL) bs_list->first = new_bs;
  else prev->next = new_bs;

  return 1;
}


int remove_bs(char *bs_id) {
  BS_Node *node = bs_list->first;
  BS_Node *prev = NULL;

  // Nothing to unregister.
  if (node == NULL) return 0;

  // Delete BS from registry.
  for (; node != NULL; prev = node, node = node->next) {
    if (strcmp(node->id, bs_id) == 0) {
      if (prev == NULL) bs_list->first = node->next;
      else prev->next = node->next;
      free(node);
      return 1;
    }
  }

  // Didn't delete.
  return 0;
}
