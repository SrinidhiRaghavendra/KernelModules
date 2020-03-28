#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include<errno.h>
int main() {
  int buffer_length = 22;
  char buffer[buffer_length];

  /*e allocate memory for character buffers HERE before you use them */

  int fd = open("/dev/proc_list", O_RDONLY);
  /* check for errors HERE */
  //int buffer_length = 64;
  ssize_t bytes_read = 1;
  int column = 0;
  while(bytes_read > 0)
  {
    bytes_read = read(fd, buffer, buffer_length);
    /* check for errors HERE. Exit loop if all processes have been retrieved. */
    /* print the output you have read so far. */
    int i = 0;
    //while(i < bytes_read) {
      /*int width = 0;
      switch(column) {
	      case 0: width = 5; break;//PID
	      case 1: width = 5; break;//PPID
	      case 2: width = 2; break;//CPUID
      	      case 3: width = 5; break;//TASK_STATE
      }
      char num[width];
      int j = 0;
      while(buffer[i+j] != ',' && buffer[i+j] != '\n') {
        num[j++] = buffer[i+j];
      }
      i++; 
      if(column == 0) {
        printf("PID=%s ", num);
      } else if(column == 1) {
        printf("PPID=%s ", num);
      } else if(column == 2) {
        printf("CPU=%s ", num);
      } else if(column == 3) {
        int int_num = atoi(num);
	*/
	switch(int_num) {
	  case 0x0000: printf("STATE=TASK_RUNNING\n"); break;
          case 0x0001: printf("STATE=TASK_INTERRUPTIBLE\n"); break;
          case 0x0002: printf("STATE=TASK_UNINTERRUPTIBLE\n"); break;
          case 0x0004: printf("STATE=TASK_STOPPED\n"); break;
          case 0x0008: printf("STATE=TASK_TRACED\n"); break;
          case 0x0040: printf("STATE=TASK_PARKED\n"); break;
          case 0x0080: printf("STATE=TASK_DEAD"); break;
          case 0x0100: printf("STATE=TASK_WAKEKILL\n"); break;
          case 0x0200: printf("STATE=TASK_WAKING\n"); break;
          case 0x0400: printf("STATE=TASK_NOLOAD\n"); break;
          case 0x0800: printf("STATE=TASK_NEW\n"); break;
          case 0x1000: printf("STATE=TASK_STATE_MAX\n"); break; 
	}
      //}
      /*i += width;
      column = (column + 1)%4;
      if(column == 0)
	      break;
      */
    //}
    //printf("%s", buffer);   
  }
  printf("\n");
  close(fd);
}
