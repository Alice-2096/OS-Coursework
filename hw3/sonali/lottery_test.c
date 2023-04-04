#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int main(int argc, char *argv[])
{   
    int test_case = atoi(argv[1]);

    printf(2, "Starting LOTTERY TEST - %d\n", test_case);
    int pid = getpid();
    printf(2, "PID of current main process: %d\n", pid);
    ps();

    int c_pid = fork ();
    printf(2, "PID of CHILD process: %d\n", c_pid);
    ps();

    printf(2, "UPDATING NICE VALUES\n");
    if (test_case == 1 && getpid() == pid)
    {
        printf(2, "PARENT MAIN PROCESS WILL FINISH FIRST: PROCESS ID: %d\n", pid);
        nice(pid, 1);
        nice(c_pid, 40);
    }
    else if (test_case == 2 && getpid() == pid)
    {
        printf(2, "CHILD PROCESS WILL FINISH FIRST: PROCESS ID: %d\n", c_pid);
        nice(pid, 40);
        nice(c_pid, 1);
    }
    else if (test_case == 3 && getpid() == pid)
    {
        printf(2, "CHILD PROCESS WILL FINISH FIRST: PROCESS ID: %d\n", c_pid);
        printf(2, "We are going to give higher priority to PARENT Process: %d but we will change its priority later to lowest.\n", pid);
        nice(pid, 1);
        nice(c_pid, 40);
    }

    ps();

    printf(2, "Starting DUMMY LOOP for DUMMY COMPUTATION for process: %d\n", getpid());
    unsigned long long int x = 0;
    for(unsigned long long int z = 0; z < 100000000; z+=1)
    {
        // if (test_case == 3 && z == 500 && getpid() == c_pid)
        if (test_case == 3 && z == 500 && getpid() == pid)
        {
            printf(2, "PROCESS CAUGHT IN IF: %d\n", getpid());
            printf(2, "getpid(): %d\n", getpid());
            printf(2, "c_pid: %d\n", c_pid);

            printf(2, "THIS IS TEST CASE 3. Updating NICE VALUE to give PARENT PROCESS LOWER PRIORITY: %d\n", pid);
            nice(pid, 40);
            nice(c_pid, 1);
            printf(2, "PS STATUS After UPDATING\n");
            ps();
        }
        x = x + (4.2);
    }
    printf(1,"DUMMY LOOP done for PROCESS: %d\n", getpid());
    printf(1,"PS AFTER COMPLETION OF LOOP: %d\n", getpid());
    ps();
    printf(1,"PROCESS FINISHED: NUMBER: %d\n", getpid());

    exit();
}