#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf(2, "Invalid Command Usage. Command template: nice -n increment/decrement (optional) command name, (optional) command arguments \n");
        exit();
    }

    // convert to int
    int n;
    int i, pos;
    pos = 1;
    if (argv[2][0] == '-')
    {
        pos = 0;
    }
    if (pos)
        n = argv[2][0] - '0';
    else
        n = 0;
    for (i = 1; i < strlen(argv[2]); i++)
    {
        n *= 10;
        n += (int)(argv[2][i] - '0');
    }
    if (!pos)
        n = -n;

    if (nice(n) != 20 && argc > 3)
        exec(argv[3], &argv[3]); // execute command after setting the nice value
    exit();
}
