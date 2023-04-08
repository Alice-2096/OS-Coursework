#include "types.h"
#include "stat.h"
#include "user.h"
int main(int argc, char *argv[])
{
    open("invalidPath", 00700);
    return 0;
}
