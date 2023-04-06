#include "types.h"
#include "stat.h"
#include "user.h"

void dummy(char *c)
{
    int z;
    int x = 0;
    for (z = 0; z < 1000000000; z += 1)
    {
        x = x + 1;
        if (z % 100000000 == 0) // print every 100000000 iteration
        {
            printf(1, c);
        }
    }
}

int main(int argc, char *argv[])
{
    printf(1, "Starting LOTTERY TEST - 4\n");

    // int c3_pid;
    int i;
    for (i = 0; i < 10; i++)
    {
        int c1_pid = fork();
        if (c1_pid == 0)
        {
            char s[] = "x";
            // child 1 runs with low priority
            nice(getpid(), 40);
            dummy(s);
            printf(1, "finished: %d\n", getpid());
            exit();
        }
        int c2_pid = fork();
        if (c2_pid == 0)
        {
            // child 2 runs with medium priority
            char s[] = "y";
            nice(getpid(), 1);
            dummy(s);
            printf(1, "finished: %d\n", getpid());
            exit();
        }
        wait();
        wait();
    }

    // else if ((c3_pid = fork()) == 0)
    // {
    //     // child 3 runs with high priority
    //     char s[] = "z";
    //     nice(getpid(), 1);
    //     dummy(s);
    //     exit();
    // }

    wait();
    wait();
    // wait();
    // child 1 will finish first

    exit();
}
