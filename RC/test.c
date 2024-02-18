#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
  FILE *fp, *sp;
  char file[128] = "10kb.txt";
  char *dot;
  char buffer[100];
  char message[100] = "RRT OK 7 0001 90115 7 hey you / 10kb.txt 1024";
  ssize_t bytes_read;
  dot = strrchr(file, '.');
  sscanf(message, "%*[^/]/ %s %s", buffer, file);
  printf("%s %s\n", buffer, file);
  return 0;
}
