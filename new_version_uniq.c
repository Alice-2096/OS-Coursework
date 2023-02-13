#include "fcntl.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

char buffer[1024];

void uniq(int fd, char *file_name, int d_param) {
  int i, n, end = 0, j, line_size = 0;
  char curr_line[1024], prev_line[1024], flag_prev[1024];
  prev_line[0] = '\0';
  flag_prev[0] = '\0';

  while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
    for (i = 0; i < n; i++) {
      if (buffer[i] == '\n' || i == n - 1) {
        for (j = end; j <= i; j++) {
          curr_line[line_size++] = buffer[j];
        }
        if (i == n - 1 && buffer[i] != '\n') {
          curr_line[line_size++] = '\n';
        }
        end = i + 1;
        if (d_param == 0) {
          if (strcmp(curr_line, prev_line) != 0) {
            printf(1, "%s", curr_line);
          }
        } else {
          if (strcmp(curr_line, prev_line) == 0) {
            strcpy(flag_prev, curr_line);
          } else {
            printf(1, "%s", flag_prev);
            flag_prev[0] = '\0';
          };
        }
        strcpy(prev_line, curr_line);
        line_size = 0;
        memset(curr_line, '\0', sizeof(curr_line));
      }
    }
    if (n < 1024)
      end = 0;
    if (n == 1024) {
      for (i = end; i < n; i++) {
        curr_line[line_size++] = buffer[i]; // Handling buffer overflow is file size > 1024
      }
    }
  }
  if(d_param==1 && strcmp(prev_line, flag_prev)==0) {
      printf(1, "%s", flag_prev);
  }
}

void print_error(char *error) {
  printf(1, "%s\n", error);
  exit();
}

int main(int argc, char *argv[]) {
  int fd, d_param = 0;
  char *file_name = "";
  if (argc > 3) {
    print_error("Enter file name and -d if required");
  }
  if (argc < 2) {
    uniq(0, file_name, d_param); // 0 is for standard input
  } else {
    int i;
    for (i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {
        if (argv[i][1] == 'd') {
          d_param = 1;
        } else {
          print_error("Incorrect parameter usage");
        }
      } else {
        file_name = argv[i];
      }
    }
  }
  if ((fd = open(file_name, O_RDONLY)) < 0) {
    printf(1, "Uniq cannot open %s\n", file_name);
    exit();
  }
  if (strcmp(file_name, "") == 0) {
    fd = 0; // If no file_name is provided then we used standard input
  }
  uniq(fd, file_name, d_param);
  exit();
}
