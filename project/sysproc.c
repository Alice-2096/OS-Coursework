#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int sys_fork(void)
{
  return fork();
}

int sys_exit(void)
{
  exit();
  return 0; // not reached
}

int sys_wait(void)
{
  return wait();
}

int sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void)
{
  return proc->pid;
}

int sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (proc->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// toggle proc->trace, flags, output file, based on user call argument
// trace(traceOnOffRun, eflag, syscallNamePtr, sflag, fflag, outputRedir, outputFilePath)
int sys_trace(void)
{
  int t; // 0 -- Trace mode OFF, 1 -- Trace mode ON
  char *syscallName, *pathName;
  if (argint(0, &t) < 0)
    return -1;
  int ef, ff, sf, redir;
  if (argint(1, &ef) < 0)
    return -1;
  if (argstr(2, &syscallName) < 0)
    return -1;
  if (argint(3, &sf) < 0)
    return -1;
  if (argint(4, &ff) < 0)
    return -1;
  if (argint(5, &redir) < 0)
    return -1;
  if (argstr(6, &pathName) < 0)
    return -1;
  if (t)
    proc->trace = 1;
  else
    proc->trace = 0;

  if (ef)
  {
    proc->eflag = 1;
    memset(proc->syscallName, 0, strlen(proc->syscallName)); // reset
    strncpy(proc->syscallName, syscallName, strlen(syscallName));
  }
  else
  {
    proc->eflag = 0;
  }

  if (sf)
    proc->sflag = 1;
  else
    proc->sflag = 0;

  if (ff)
    proc->fflag = 1;
  else
    proc->fflag = 0;

  if (redir)
  {
    proc->redir = 1;
    strncpy(proc->dirPath, pathName, strlen(pathName));
  }
  else
    proc->redir = 0;
  return 0;
}

int sys_dump(void)
{
  // do nothing
  return 0;
}
