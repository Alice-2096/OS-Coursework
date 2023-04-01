#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
    printf(2, "TESTING FOR RANDOM FUNCTION HEALTH FOR MAX VALUE OF 10\n\n");
    int temp_random_value;
    long int total_sum = 0;

    int count_1 = 0;
    int count_2 = 0;
    int count_3 = 0;
    int count_4 = 0;
    int count_5 = 0;
    int count_6 = 0;
    int count_7 = 0;
    int count_8 = 0;
    int count_9 = 0;
    int count_10 = 0;

    for(int i = 0; i < 10000; i++)
    {   
        temp_random_value = random(10);
        total_sum += temp_random_value;

        if(temp_random_value == 1){count_1++ ;}
        else if(temp_random_value == 2){count_2++ ;}
        else if(temp_random_value == 3){count_3++ ;}
        else if(temp_random_value == 4){count_4++ ;}
        else if(temp_random_value == 5){count_5++ ;}
        else if(temp_random_value == 6){count_6++ ;}
        else if(temp_random_value == 7){count_7++ ;}
        else if(temp_random_value == 8){count_8++ ;}
        else if(temp_random_value == 9){count_9++ ;}
        else if(temp_random_value == 10){count_10++ ;}
        else
        {
            printf(2, "EXTRA ERROR VALUE: %d\n", temp_random_value);
        }
    }

    printf(2, "Total Sum of 10000 iterations: %d\n", total_sum);
    printf(2, "Number of occurences of digit 1: %d\n", count_1);
    printf(2, "Number of occurences of digit 2: %d\n", count_2);
    printf(2, "Number of occurences of digit 3: %d\n", count_3);
    printf(2, "Number of occurences of digit 4: %d\n", count_4);
    printf(2, "Number of occurences of digit 5: %d\n", count_5);
    printf(2, "Number of occurences of digit 6: %d\n", count_6);
    printf(2, "Number of occurences of digit 7: %d\n", count_7);
    printf(2, "Number of occurences of digit 8: %d\n", count_8);
    printf(2, "Number of occurences of digit 9: %d\n", count_9);
    printf(2, "Number of occurences of digit 10: %d\n", count_10);

    exit();
}