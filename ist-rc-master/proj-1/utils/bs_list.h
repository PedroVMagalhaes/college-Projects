#ifndef __BS_LIST_H__
#define __BS_LIST_H__

#include "generic.h"

void load_bs_list();
void clean_bs_list();
int add_bs(char *ip_port);
int remove_bs(char *bs_id);

#endif
