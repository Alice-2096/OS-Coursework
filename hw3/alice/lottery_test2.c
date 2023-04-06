#include "types.h"
#include "date.h"
#include "user.h"

void dummy()
{
    struct rtcdate ending;
    int pid = getpid();

    unsigned long long int x = 0;
    unsigned long long int z;
    for (z = 0; z < 10000000000000; z += 1)
    {
        x = x + 1;
    }
    gettime(&ending);

    printf(2, "%d has finished | %d:%d:%d\n", pid, ending.hour, ending.minute, ending.second);
}

int main(int argc, char *argv[])
{
    printf(2, "Starting LOTTERY TEST - 4\n");
    struct rtcdate start;
    int c1 = fork();
    if (c1 == 0)
    {
        c1 = getpid();
        nice(c1, 40);
        gettime(&start);
        printf(1, "%d starts | %d:%d:%d\n", c1, start.hour, start.minute, start.second);

        dummy();
        exit();
    }
    int c2 = fork();
    if (c2 == 0)
    {
        c2 = getpid();
        // child 2 runs here with higher priority
        nice(c2, 1);
        gettime(&start);
        printf(1, "%d starts | %d:%d:%d\n", c2, start.hour, start.minute, start.second);

        dummy();
        exit();
    }

    wait();
    wait();
    // output shows that child 2 will finish first

    exit();
}
