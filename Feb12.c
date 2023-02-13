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
            // printf(1, "MALLOC, bufsize is now%d\n", bufSize);
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

//-c flag --> prefix==1, otherwise prefix==0
//-i flag --> ignore==1, otherwise ignore==0
//-d flag --> printDup==1, otherwise printDup==0
void uniq(int fd, int prefix, int ignore, int printDup)
{
    int n, cnt = 1;
    char *buf = readToBuf(fd);
    n = strlen(buf) + 1;
    // printf(1, "total num of chars is %d\n", n);
    int start = 0;
    int lenA = 0, lenB = 0;
    char *sA = buf, *sB = buf; // sA prevline, sB currline

    while (start < n && buf[start] != '\0')
    {
        // searches for the first occurrence of '\n'
        char *ptr = strchr(sB, '\n');
        if (!ptr)
            ptr = buf + (n - 1);
        lenB = ptr - sB;
        // printf(1, "sB's length: %d", lenB);

        // compare sA(prevline) and sB(currline)
        if (start != 0)
        {
            if ((!ignore && strcmp1(sA, sB, lenA, lenB) == 0) || (ignore && strcmp2(sA, sB, lenA, lenB) == 0))
            {
                // if same
                cnt++;
            }
            else
            {
                // if different, print according to flags
                char *line = malloc(sizeof(char) * (lenA + 1));
                memmove(line, sA, lenA);
                line[lenA] = '\0';
                if (!printDup && prefix)
                    printf(1, "%d %s\n", cnt, line);
                else if (!printDup && !prefix)
                    printf(1, "%s\n", line);
                else if (printDup && cnt > 1)
                    printf(1, "%s\n", line);
                free(line);
                cnt = 1;
                sA = sB;
                lenA = lenB;
            }
        }
        else
        {
            cnt = 1;
            sA = buf;
            lenA = lenB;
        }
        start = ptr - buf + 1;
        sB = ptr + 1;
    }
    // print the last line
    char *line = malloc(sizeof(char) * (lenA + 1));
    memmove(line, sA, lenA);
    line[lenA] = '\0';
    if (!printDup && prefix)
        printf(1, "%d %s\n", cnt, line);
    else if (!printDup && !prefix)
        printf(1, "%s\n", line);
    else if (printDup && cnt > 1)
        printf(1, "%s\n", line);
    free(line);
    return;
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
