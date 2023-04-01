#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int nice_value, pid;
  if(argc != 3){
    printf(2,"Invalid Command Usage. Command template: nice <Target Process ID> <Nice Value>\n");
    exit();
  }

  pid = atoi(argv[1]);
  nice_value = atoi(argv[2]);
  
  nice(pid, nice_value);
  exit();
}