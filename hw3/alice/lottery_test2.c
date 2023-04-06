#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

void dummy()
{
    int z;
    int x = 0;
    double y = 2.0;
    for (z = 0; z < 1000000; z += 1)
    {
        x = x + 1;
        y = y * (double)(x - 123432);
        y = 2.0;
    }
}

// command syntax is: 1st child nice value, 2nd child nice value
int main(int argc, char *argv[])
{
    printf(1, "Starting LOTTERY TEST - 2\n");
    int f, s;
    if (argc < 3)
    {
        printf(2, "invalid command syntax.\n");
        exit();
    }
    f = atoi(argv[1]);
    s = atoi(argv[2]);

    int i;
    for (i = 0; i < 10; i++)
    {
        int c1_pid = fork();
        if (c1_pid == 0)
        {
            nice(getpid(), f);
            dummy();
            printf(1, "child 1 with nice value %d has finished | pID is: %d\n", f, getpid());
            exit();
        }
        int c2_pid = fork();
        if (c2_pid == 0)
        {
            nice(getpid(), s);
            dummy();
            printf(1, "child 2 with nice value %d has finished | pID is: %d\n", s, getpid());
            exit();
        }
        wait();
        wait();
        printf(1, "\n");
    }

    exit();
}
