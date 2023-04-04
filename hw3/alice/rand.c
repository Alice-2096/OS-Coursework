#include "types.h"
#include "stat.h"
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
    uint seed = 12345;
    int i, j;
    int histogram[20] = {0};

    // generate 1000 random numbers in the range [0,49]
    for (i = 0; i < 1000; i++)
    {
        seed = xorshift(seed); // Generate the next random number
        int bin = seed % 20;
        histogram[bin]++;
    }

    // Print histogram
    for (i = 0; i < 20; ++i)
    {
        printf(1, "%d: ", i);
        for (j = 0; j < histogram[i]; j++)
        {
            printf(1, "*");
        }
        printf(1, "\n");
    }
    printf(1, "\nProbability of each bin:\n");
    for (i = 0; i < 20; i++)
    {
        int pb = histogram[i] / 10;
        printf(1, "%d: %d%\n", i, pb); // not sure how to print double
    }

    exit();
}
