#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

void dummy()
{
    unsigned long long int z;
    unsigned long long int x = -10;
    for (z = 0; z < 100000000; z += 1)
    {
        x = x + 1;
    }
}

int main(int argc, char *argv[])
{
    printf(1, "Starting LOTTERY TEST - 5\n");
    int f, s, t;
    if (argc < 4)
    {
        printf(2, "invalid command syntax.\n");
        exit();
    }
    f = atoi(argv[1]);
    s = atoi(argv[2]);
    t = atoi(argv[3]);

    int c1_pid = fork();
    if (c1_pid == 0)
    {
        nice(getpid(), f);
        int c2_pid = fork();
        if (c2_pid == 0)
        {
            nice(getpid(), s);
            int c3_pid = fork();
            if (c3_pid == 0)
            {
                nice(getpid(), t);
                dummy();
                printf(1, "child 3 with nice value %d has finished | pID is: %d\n", t, getpid());
                exit();
            }
            dummy();
            printf(1, "child 2 with nice value %d has finished | pID is: %d\n", s, getpid());
            wait();
            exit();
        }
        dummy();
        printf(1, "child 1 with nice value %d has finished | pID is: %d\n", f, getpid());
        wait();
        exit();
    }

    wait();
    exit();
}
