#include "types.h"
#include "stat.h"
#include "user.h"

#define MAXLENGTH 1000

char buf[MAXLENGTH];
char currline[MAXLENGTH];
char nextline[MAXLENGTH]; //! FIX: CANNOT HANDLE LONGER LINES

// util function: compare string ignoring case difference
// return 0 if same
int strcmp2(char *a, char *b)
{
    if (strlen(a) != strlen(b))
        return 1;
    else
    {
        int i;
        for (i = 0; i < strlen(a); i++)
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

//-c flag --> prefix==1, otherwise prefix==0
//-i flag --> ignore==1, otherwise ignore==0
//-d flag --> printDup==1, otherwise printDup==0
void uniq(int fd, int prefix, int ignore, int printDup)
{
    int n, j, cnt = 1;

    while ((n = read(fd, buf, sizeof(buf))) > 0)
    {
        int start = 0;
        while (start < n)
        {
            // searches for the first occurrence of '\n'
            char *ptr = strchr(buf + start, '\n');
            if (!ptr)
                ptr = buf + n - 1;
            // copy into nextline
            int ind = 0;
            memset(nextline, '\0', sizeof(nextline));
            for (j = start; j <= ptr - buf; j++, ind++)
                nextline[ind] = buf[j];
            // compare currline and nextline
            if ((!ignore && strcmp(nextline, currline) == 0) || (ignore && strcmp2(nextline, currline) == 0))
                cnt++; // if they are the same line
            else
            {
                // if they are different
                if (start == 0)
                {
                }
                else if (!printDup && prefix)
                    printf(1, "%d %s", cnt, currline);
                else if (!printDup && !prefix)
                    printf(1, "%s", currline);
                else if (printDup && cnt > 1)
                    printf(1, "%s", currline);

                // reset
                cnt = 1;
                strcpy(currline, nextline);
            }
            start = ptr - buf + 1;
        }
        // print the last line
        if (prefix)
            printf(1, "%d %s\n", cnt, currline);
        else if (!printDup || cnt > 1)
            printf(1, "%s\n", currline);
    }

    if (n < 0)
    {
        printf(1, "uniq: read error\n");
        exit();
    }
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
