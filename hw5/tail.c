#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char buf[1024];

void tail(int fd, int num)
{
    int i, n;
    int tot_lines = 0;

    int fd2 = open("buffer", O_CREATE | O_RDWR);

    while ((n = read(fd, buf, sizeof(buf))) > 0)
    {
        write(fd2, buf, n);
        // calculate total number of lines
        for (i = 0; i < n; i++)
        {
            if (buf[i] == '\n')
            {
                tot_lines++;
            }
        }
    }

    int count = 0;
    // calculate which line to start printing
    int start = tot_lines - num;
    // printf(1, "total lines: %d | start from line: %d\n", tot_lines, start);

    // Reset the file pointer to the beginning of the file by closing the file and reopen it
    close(fd2);
    if ((fd2 = open("buffer", 0)) < 0)
    {
        printf(1, "tail: cannot open temporary buffer.\n");
        exit();
    }
    while ((n = read(fd2, buf, sizeof(buf))) > 0)
    {
        buf[n] = '\0'; // Add null terminator
        for (i = 0; i < n; i++)
        {
            if (count >= start)
            {
                printf(1, "%s", buf + i);
                break;
            }
            else if (buf[i] == '\n')
            {
                count++;
            }
        }
    }
    // remove from file system
    close(fd2);
    unlink("buffer");
}

int main(int argc, char *argv[])
{
    int i;
    int fd = 0, num = 10; // default

    if (argc == 1)
    {
        tail(0, 10);
        exit();
    }
    else
    {
        for (i = 1; i < argc; i++)
        {
            if (argv[i][0] == '-')
            {
                // handle number
                num = atoi(++argv[i]);
            }
            else
            {
                // handle file
                if ((fd = open(argv[i], 0)) < 0)
                {
                    printf(1, "tail: cannot open %s\n", argv[i]);
                    exit();
                }
            }
        }
        tail(fd, num);
        close(fd);
        exit();
    }
}
