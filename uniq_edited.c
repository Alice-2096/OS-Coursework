#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define BUFSIZE 1000

int main(int argc, char *argv[]) {
  int fd, n;
  char buf[BUFSIZE], prev[BUFSIZE];

  if(argc <= 1) {
    printf(2, "Usage: uniq file\n");
    exit();
  }

  if((fd = open(argv[1], O_RDONLY)) < 0) {
    printf(2, "uniq: cannot open %s\n", argv[1]);
    exit();
  }

  n = read(fd, buf, BUFSIZE);
  strcpy(prev, buf);

  while(n > 0) {
    n = read(fd, buf, BUFSIZE);
    if(n < 0) {
      printf(2, "uniq: read error\n");
      exit();
    }

    if(strcmp(buf, prev) != 0) {
      printf(1, "%s", prev);
      strcpy(prev, buf);
    }
  }

  printf(1, "%s", prev);
  close(fd);
  exit();
}
