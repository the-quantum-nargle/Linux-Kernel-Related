#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
  int fd;
  int count;
  char buf[100];
  char* params = argv[1];

  fd = open("/dev/helloworld", O_RDWR);
  if (fd == -1){
    printf("Fail to open device helloworld.\n");
    exit(-1);
  }

  count = write(fd, params, 250);

  close(fd);
  return 0;
}
  

