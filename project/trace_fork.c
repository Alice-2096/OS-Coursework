#include "types.h"
#include "stat.h"
#include "user.h"
int main(int argc, char *argv[])
{
    if (fork() == 0)
    {
        printf(1, "C\n");
        if (fork() == 0)
        {
            printf(1, "G\n");
            exit();
        }
        wait();
        exit();
    }
    wait();
    printf(1, "P\n");
    exit();
}
