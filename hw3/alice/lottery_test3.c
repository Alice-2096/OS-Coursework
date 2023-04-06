#include "types.h"
#include "stat.h"
#include "user.h"

void dummy(char *c)
{
    int z;
    for (z = 0; z < 100000000; z += 1)
    {
        if (z + 1 == 100000000)
        {
            printf(1, "F\n"); // F indicates finish
        }
        else if (z % 1000000 == 0)
        {
            printf(1, c);
        }
    }
}

int main(int argc, char *argv[])
{
    printf(2, "Starting LOTTERY TEST - 4\n");

    int c1_pid;
    int c2_pid;
    int c3_pid;

    if ((c1_pid = fork()) == 0)
    {
        char s[] = "x";
        // child 1 runs with high priority
        nice(c1_pid, 1);
        dummy(s);
        exit();
    }
    else if ((c2_pid = fork()) == 0)
    {
        // child 2 runs with medium priority
        char s[] = "y";
        nice(c2_pid, 20);
        dummy(s);
        exit();
    }
    else if ((c3_pid = fork()) == 0)
    {
        // child 3 runs with low priority
        char s[] = "z";
        nice(c3_pid, 40);
        ps();
        dummy(s);
        exit();
    }
    wait();
    wait();
    wait();
    // child 1 will finish first

    exit();
}
