#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

uint xorshift(uint x)
{
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

int main(int argc, char *argv[])
{
    printf(1, "TESTING FOR XORSHIFT FOR MAX VALUE OF 10\n\n");
    int temp_random_value;
    // long int total_sum = 0;
    int count[10] = {0};

    uint seed = 52396; // initialize the seed

    int i;
    for (i = 0; i < 10000; i++)
    {

        seed = xorshift(seed);
        temp_random_value = (seed % 10) + 1;

        if (temp_random_value >= 1 && temp_random_value <= 10)
        {
            count[temp_random_value - 1]++;
        }
        else
        {
            printf(2, "EXTRA ERROR VALUE: %d\n", temp_random_value);
        }
    }

    // Print histogram
    printf(1, " -- HISTOGRAM -- \n");
    for (i = 0; i < 10; ++i)
    {
        printf(1, "%d: ", i + 1);
        int j;
        for (j = 0; j < count[i] / 50; j++)
        {
            printf(1, "*");
        }
        printf(1, "\n");
    }

    printf(1, "\n");
    for (i = 0; i < 10; ++i)
    {
        printf(1, "Number of occurences of digit %d: %d\n", i + 1, count[i]);
    }

    exit();
}
