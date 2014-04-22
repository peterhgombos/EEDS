#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int fd;
  char ch, buf[10];

  printf("Hello World, I'm game!\n");

  fd = open ("/dev/gamepad", O_RDONLY);
  if (fd == -1) {
      printf ("Error opening device\n");
      exit (-1);
  }

  read (fd, buf, sizeof(buf));
  printf("Buffer says: %s\n", buf);

  if (close(fd) != 0) {
      printf("Error closing device\n");
  }


  exit (EXIT_SUCCESS);
}
