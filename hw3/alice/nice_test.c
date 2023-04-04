#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
    int n, nv;
    int i, pos;
    pos = 1;
    if (argc < 2)
        exit();
    if (argv[1][0] == '-')
    {
        pos = 0;
    }
    if (pos)
        n = argv[1][0] - '0';
    else
        n = 0;
    for (i = 1; i < strlen(argv[1]); i++)
    {
        n *= 10;
        n += (int)(argv[1][i] - '0');
    }
    if (!pos)
        n = -n;

    printf(1, "increment or decrement value %d: \n", n);
    nv = nice(n);
    if (nv != 20)
        printf(1, "the updated nice value: %d \n", nv);
    else
    {
        // do nothing;
    }
    exit();
}
