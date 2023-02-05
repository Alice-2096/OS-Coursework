#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define BUFSIZE 512

int strcmp2(char *s1, char *s2) {
  int i;
  for (i = 0; s1[i] && s2[i]; i++) {
    if (s1[i] != s2[i]) {
      return s1[i] - s2[i];
    }
  }
  return s1[i] - s2[i];
}

int main(int argc, char *argv[]) {
  int fd, n;
  char buf[BUFSIZE];
  char prev[BUFSIZE];

  if(argc == 1) {
    fd = 0; // use standard input
  } else if(argc == 2) {
    if((fd = open(argv[1], O_RDONLY)) < 0) {
      printf(2, "uniq: cannot open %s\n", argv[1]);
      exit();
    }
  } else {
    printf(2, "Usage: uniq [file]\n");
    exit();
  }

  int prevlen = 0;
  while((n = read(fd, buf, BUFSIZE - 1)) > 0) {
    int i, j;
    j = 0;
    for (i = 0; i < n; i++) {
      if (buf[i] == '\n' || i == n - 1) {
        buf[i] = 0;
        if (prevlen == 0 || strcmp2(buf + j, prev) != 0) {
          printf(1, "%s\n", buf + j);
          memmove(prev, buf + j, i - j + 1);
          prevlen = i - j + 1;
        }
        j = i + 1;
      }
    }
  }

  if(argc == 2) {
    close(fd);
  }
  exit();
}
