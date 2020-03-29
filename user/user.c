#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include<errno.h>

int main() {
  int buffer_length = 64;
  char buffer[buffer_length];
  int fd = open("/dev/proc_list", O_RDONLY);
  int fd2 = open("/dev/proc_list", O_RDONLY);
  /* check for errors HERE */
  ssize_t bytes_read = buffer_length;
  while(bytes_read == buffer_length)
  {
    bytes_read = read(fd, buffer, buffer_length);
    printf("%.*s", (int)bytes_read, buffer);
  }
  bytes_read = buffer_length;
  printf("_______Divider__________\n_");
  while(bytes_read == buffer_length)
  {
    bytes_read = read(fd2, buffer, buffer_length);
    printf("%.*s", (int)bytes_read, buffer);
  }
  close(fd);
  close(fd2);
}
