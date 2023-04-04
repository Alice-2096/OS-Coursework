#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
// #include "kernel/fcntl.h"

int main(int argc, char *argv[])
{
    printf(2, "\nRunning NICE TESTS\n");
    int pid = getpid();
    printf(2, "Process ID of this Process: %d\n", pid);
    printf(2, "\nPS COMMAND OUTPUT - DEFAULT PROCESSES\n");
    ps();

    printf(2, "\n\nUpdating NICE VALUE to LOWER PRIORITY VALUE LIKE 32\n");
    printf(2, "PS COMMAND OUTPUT - AFTER NICE CALL FOR ABOVE OPERATION\n");
    nice(pid, 32);
    ps();

    printf(2, "\n\nUpdating NICE VALUE to HIGHER PRIORITY VALUE LIKE 8\n");
    printf(2, "PS COMMAND OUTPUT - AFTER NICE CALL FOR ABOVE OPERATION\n");
    nice(pid, 8);
    ps();

    printf(2, "\n\nUpdating NICE VALUE to LOWER OUT OF BOUND VALUE LIKE 50\n");
    printf(2, "PS COMMAND OUTPUT - AFTER NICE CALL FOR ABOVE OPERATION\n");
    nice(pid, 50);
    ps();

    printf(2, "\n\nUpdating NICE VALUE to HIGHER OUT OF BOUND VALUE LIKE -5\n");
    printf(2, "PS COMMAND OUTPUT - AFTER NICE CALL FOR ABOVE OPERATION\n");
    nice(pid, -5);
    ps();

    exit();
}