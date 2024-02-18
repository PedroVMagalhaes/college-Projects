#ifndef __AUTH_H__
#define __AUTH_H__

#include "generic.h"

int verify_user(char *username, char *password);
int create_user(char *username, char *password);
int load_user(char *username, char *password);
void clean_user();

#endif
