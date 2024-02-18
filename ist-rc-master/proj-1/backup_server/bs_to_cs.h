#ifndef __BS_TO_CS_H__
#define __BS_TO_CS_H__

#include "../utils/generic.h"
#include "../utils/coms.h"
#include "../utils/file_utils.h"
#include "../utils/command_utils.h"
#include "../utils/auth.h"

// Commands
#define LSF_CMD "LSF"
#define LSU_CMD "LSU"
#define DLB_CMD "DLB"

int register_with_cs(unsigned short int port);
void cs_handler(unsigned short int port);
void cs_exec_command(int fd, struct sockaddr_in addr, socklen_t s_len);

#endif
