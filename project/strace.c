#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
    if (argc == 2 && !strcmp(argv[1], "on"))
    {
        trace(1);
    }
    else if (argc == 2 && !strcmp(argv[1], "off"))
    {
        trace(0);
    }
    else
    {
        printf(2, "strace command error!\n");
        exit();
    }
    exit();
}
