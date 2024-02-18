CC       = gcc
CFLAGS   = -g -std=c99 -Wall -pedantic -pthread

.PHONY: all clean zip

all: user CS BS

user: user_client/main.c user_client/user.c utils/generic.c utils/command_utils.c utils/file_utils.c
	$(CC) $(CFLAGS) -o $@ $+

CS: central_server/main.c central_server/cs_to_user.c user_client/user.c utils/command_utils.c central_server/cs_to_bs.c utils/bs_list.c utils/auth.c utils/coms.c utils/generic.c utils/file_utils.c
	$(CC) $(CFLAGS) -o $@ $+

BS: backup_server/main.c backup_server/bs_to_user.c backup_server/bs_to_cs.c user_client/user.c utils/auth.c utils/coms.c utils/generic.c utils/command_utils.c utils/file_utils.c
	$(CC) $(CFLAGS) -o $@ $+

zip: proj_34.zip

proj_34.zip: Makefile user_client/ central_server/ backup_server/ utils/
	zip -r $@ $+

clean:
	rm -rf user CS BS *.dSYM *.o *.zip
