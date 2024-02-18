CC = gcc
RM = rm -f

all: client_prg server_prg

client_prg: client.c
	$(CC) -o user client.c

server_prg: server.c
	$(CC) -o DS server.c

clean:
	$(RM) user DS

