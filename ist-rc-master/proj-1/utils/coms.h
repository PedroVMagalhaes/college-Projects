#ifndef __COMS_H__
#define __COMS_H__

#include "generic.h"

/**
 * Configure socket address.
 *
 * @author Group 34
 * @param  port Port number for the socket.
 * @return Socket address.
 */
struct sockaddr_in config_addr(char *host, unsigned short int port);

int open_tcp_socket();
int open_udp_socket();

/**
 * Start an UDP server.
 *
 * @author Group 34
 * @param  addr Socket address.
 * @return Socket file descriptor.
 */
int start_udp_server(struct sockaddr_in addr);

/**
 * Start an TCP server.
 *
 * @author Group 34
 * @param  addr Socket address.
 * @return Socket file descriptor.
 */
int start_tcp_server(struct sockaddr_in addr);

char *get_host_ip(char *hostname);
int udp_send(int fd, struct sockaddr_in *addr, char *msg);
int udp_receive(int fd, struct sockaddr_in *addr, char **response);

int tcp_send(int fd, char *response);
int tcp_receive(int fd, char **cmd_line);

#endif
