#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include<errno.h>
int main() {
  char buffer[64];

  /*e allocate memory for character buffers HERE before you use them */

  int fd = open("/dev/proc_list", O_RDONLY);
  /* check for errors HERE */
  int buffer_length = 64;
  ssize_t bytes_read = 1;
  while(bytes_read > 0)
  {
    bytes_read = read(fd, buffer, buffer_length);
    /* check for errors HERE. Exit loop if all processes have been retrieved. */
    /* print the output you have read so far. */
    printf("%s", buffer);   
  }

  close(fd);
}
