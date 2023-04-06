#include "types.h"
#include "date.h"
#include "user.h"

void dummy()
{
    struct rtcdate ending;
    int pid = getpid();
    // printf(2, "Starting loop for process %d", pid);

    unsigned long long int x = 0;
    unsigned long long int z;
    for (z = 0; z < 100000000; z += 1)
    {
        x = x + 1;
    }
    gettime(&ending);

    printf(2, "%d has finished at %d:%d:%d seconds\n", pid, nice, ending.hour, ending.minute, ending.second);
}

int main(int argc, char *argv[])
{
    printf(2, "Starting LOTTERY TEST - 4\n");

    int c1_pid;
    int c2_pid;

    if ((c1_pid = fork()) == 0)
    {
        // child 1 runs with low priority
        nice(c1_pid, 40);
        dummy();
        exit();
    }
    else if ((c2_pid = fork()) == 0)
    {
        // child 2 runs here with higher priority
        nice(c2_pid, 1);
        dummy();
        exit();
    }

    wait();
    wait();
    // output shows that child 2 will finish first

    exit();
}
