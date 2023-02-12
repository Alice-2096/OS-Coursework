#include "types.h"
#include "stat.h"
#include "user.h"

// util function: compare string ignoring case difference
// return 0 if same
int strcmp2(char *a, char *b, int lenA, int lenB)
{
    if (lenA != lenB)
        return 1;
    else
    {
        int i;
        for (i = 0; i < lenA; i++)
        {
            // transform into lowercase
            char fst = a[i], sec = b[i];
            if (a[i] >= 'A' && a[i] <= 'Z')
                fst = 'a' + (a[i] - 'A');
            if (b[i] >= 'A' && b[i] <= 'Z')
                sec = 'a' + (b[i] - 'A');
            if (fst != sec)
                return 1;
        }
        return 0;
    }
}
int strcmp1(char *a, char *b, int lenA, int lenB)
{
    if (lenA != lenB)
        return 1;
    else
    {
        int i;
        for (i = 0; i < lenA; i++)
        {
            if (a[i] != b[i])
                return 1;
        }
        return 0;
    }
}

// given an open file, read file to buffer, return num of bytes read
// THIS PART IS ALL GOOD. 
char *readToBuf(int fd)
{
    int n, currPos = 0, bufSize = 1024;
    char *buf = malloc(sizeof(char) * 1024);
    while ((n = read(fd, buf + currPos, 1024)) > 0)
    {
        currPos += n;
        if (n < 1024)
        {
            // reach eof
            buf[currPos] = '\0';
            break;
        }
        else
        {
            // buffer resize
            bufSize += 1024;
            char *newBuf = malloc(sizeof(char) * bufSize);
            printf(1, "MALLOC, bufsize is now%d\n", bufSize);
            if (!newBuf)
            {
                printf(1, "memory allocation error\n");
                exit();
            }
            memmove(newBuf, buf, currPos);
            free(buf);
            buf = newBuf;
        }
    }
    if (n < 0)
    {
        printf(1, "uniq: read error\n");
        exit();
    }
    return buf;
}

//! BUGS 
//-c flag --> prefix==1, otherwise prefix==0
//-i flag --> ignore==1, otherwise ignore==0
//-d flag --> printDup==1, otherwise printDup==0
void uniq(int fd, int prefix, int ignore, int printDup)
{
    int n, cnt = 1;
    char *buf = readToBuf(fd);
    n = strlen(buf) + 1;
    printf(1, "total num of chars is %d\n", n);
    int start = 0;
    int lenA = 0, lenB = 0;
    char *sA = buf, *sB = buf; // sA currline, sB nextline

    while (start < n)
    {
        // searches for the first occurrence of '\n'
        char *ptr = strchr(buf + start, '\n');
        if (!ptr)
            ptr = buf + (n - 1);
        lenB = ptr - buf;
        printf(1, "sB's length: %d", lenB);

        // compare currline and nextline
        if (start != 0)
        {
            if ((!ignore && strcmp1(sA, sB, lenA, lenB) == 0) || (ignore && strcmp2(sA, sB, lenA, lenB) == 0))
            {
                // if same
                cnt++;
                printf(1, "same string\n");
            }
            else
            {
                // if different, print according to flags
                printf(1, "different string, print sA:\n");
                printf(1, "(%.*s)\n", lenA, sA);
                // if (!printDup && prefix)
                //     printf(1, "%d %s", cnt, currline);
                // else if (!printDup && !prefix)
                //     printf(1, "%s", currline);
                // else if (printDup && cnt > 1)
                //     printf(1, "%s", currline);
                cnt = 1;
                memmove(sA, sB, lenB);
            }
        }
        else
        {
            cnt = 1;
            memmove(sA, sB, lenB);
        }
        printf(1, "after moving nextline to currline\n");
        start = ptr - buf + 1;
        printf(1, "reset start to %d\n", start);
        sA = sB;
        lenA = lenB;
        sB = buf + start;
    }
    // print the last line
    // if (prefix)
    //     printf(1, "%d %s\n", cnt, currline);
    // else if (!printDup || cnt > 1)
    //     printf(1, "%s\n", currline);
    // printf(1, "line124\n");
}

int main(int argc, char *argv[])
{
    int fd = 0, i;
    int prefix = 0, ignore = 0, printDup = 0;

    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            // flag
            if (argv[i][1] == 'c')
                prefix = 1;
            if (argv[i][1] == 'd')
                printDup = 1;
            if (argv[i][1] == 'i')
                ignore = 1;
        }
        else
        {
            // fileName
            if ((fd = open(argv[i], 0)) < 0)
            {
                printf(1, "uniq: cannot open %s\n", argv[i]);
                exit();
            }
            else
            {
                uniq(fd, prefix, ignore, printDup);
                close(fd);
                exit();
            }
        }
    }
    uniq(0, prefix, ignore, printDup);
    exit();
}
