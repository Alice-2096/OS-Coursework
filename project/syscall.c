#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "syscall.h"

// User code makes a system call with INT T_SYSCALL.
// System call number in %eax.
// Arguments on the stack, from the user call to the C
// library system call function. The saved user %esp points
// to a saved program counter, and then the first argument.

// Fetch the int at addr from the current process.
int fetchint(uint addr, int *ip)
{
  if (addr >= proc->sz || addr + 4 > proc->sz)
    return -1;
  *ip = *(int *)(addr);
  return 0;
}

// Fetch the nul-terminated string at addr from the current process.
// Doesn't actually copy the string - just sets *pp to point at it.
// Returns length of string, not including nul.
int fetchstr(uint addr, char **pp)
{
  char *s, *ep;

  if (addr >= proc->sz)
    return -1;
  *pp = (char *)addr;
  ep = (char *)proc->sz;
  for (s = *pp; s < ep; s++)
    if (*s == 0)
      return s - *pp;
  return -1;
}

// Fetch the nth 32-bit system call argument.
int argint(int n, int *ip)
{
  return fetchint(proc->tf->esp + 4 + 4 * n, ip);
}

// Fetch the nth word-sized system call argument as a pointer
// to a block of memory of size bytes.  Check that the pointer
// lies within the process address space.
int argptr(int n, char **pp, int size)
{
  int i;

  if (argint(n, &i) < 0)
    return -1;
  if (size < 0 || (uint)i >= proc->sz || (uint)i + size > proc->sz)
    return -1;
  *pp = (char *)i;
  return 0;
}

// Fetch the nth word-sized system call argument as a string pointer.
// Check that the pointer is valid and the string is nul-terminated.
// (There is no shared writable memory, so the string can't change
// between this check and being used by the kernel.)
int argstr(int n, char **pp)
{
  int addr;
  if (argint(n, &addr) < 0)
    return -1;
  return fetchstr(addr, pp);
}

extern int sys_chdir(void);
extern int sys_close(void);
extern int sys_dup(void);
extern int sys_exec(void);
extern int sys_exit(void);
extern int sys_fork(void);
extern int sys_fstat(void);
extern int sys_getpid(void);
extern int sys_kill(void);
extern int sys_link(void);
extern int sys_mkdir(void);
extern int sys_mknod(void);
extern int sys_open(void);
extern int sys_pipe(void);
extern int sys_read(void);
extern int sys_sbrk(void);
extern int sys_sleep(void);
extern int sys_unlink(void);
extern int sys_wait(void);
extern int sys_write(void);
extern int sys_uptime(void);
extern int sys_trace(void);
extern int sys_dump(void);

static int (*syscalls[])(void) = {
    [SYS_fork] sys_fork,
    [SYS_exit] sys_exit,
    [SYS_wait] sys_wait,
    [SYS_pipe] sys_pipe,
    [SYS_read] sys_read,
    [SYS_kill] sys_kill,
    [SYS_exec] sys_exec,
    [SYS_fstat] sys_fstat,
    [SYS_chdir] sys_chdir,
    [SYS_dup] sys_dup,
    [SYS_getpid] sys_getpid,
    [SYS_sbrk] sys_sbrk,
    [SYS_sleep] sys_sleep,
    [SYS_uptime] sys_uptime,
    [SYS_open] sys_open,
    [SYS_write] sys_write,
    [SYS_mknod] sys_mknod,
    [SYS_unlink] sys_unlink,
    [SYS_link] sys_link,
    [SYS_mkdir] sys_mkdir,
    [SYS_close] sys_close,
    [SYS_trace] sys_trace,
    [SYS_dump] sys_dump,
};

static char *syscalls_names[] = {
    [SYS_fork] "fork",
    [SYS_exit] "exit",
    [SYS_wait] "wait",
    [SYS_pipe] "pipe",
    [SYS_read] "read",
    [SYS_kill] "kill",
    [SYS_exec] "exec",
    [SYS_fstat] "fstat",
    [SYS_chdir] "chdir",
    [SYS_dup] "dup",
    [SYS_getpid] "getpid",
    [SYS_sbrk] "sbrk",
    [SYS_sleep] "sleep",
    [SYS_uptime] "uptime",
    [SYS_open] "open",
    [SYS_write] "write",
    [SYS_mknod] "mknod",
    [SYS_unlink] "unlink",
    [SYS_link] "link",
    [SYS_mkdir] "mkdir",
    [SYS_close] "close",
    [SYS_trace] "trace",
    [SYS_dump] "dump",
};

static char *events_cmd_name[DUMPSIZE];
static char *events_syscall[DUMPSIZE];
static int events_return_value[DUMPSIZE];
static int events_pid[DUMPSIZE];
static int full = 0;
static int curr = 0;

// TODO take in the index where exit syscall of the current program is stored --> curr - 1
// TODO backtracking all the way to the first syscall.
// TODO then prints from first to exit
void printEvents(int pid)
{
  int start = full ? curr : 0;
  int i, j = 0;
  for (i = start; j < DUMPSIZE; j++)
  {
    cprintf("DUMP: pid = %d | command name = %s | syscall = %s | return value = %d\n ", events_pid[i], events_cmd_name[i], events_syscall[i], events_return_value[i]);

    if (i < DUMPSIZE - 1)
      i++;
    else
      i = 0;
  }
}

void syscall(void)
{
  int num = proc->tf->eax;
  if (num > 0 && num < NELEM(syscalls) && syscalls[num])
  {
    if (num == SYS_dump)
    {
      printEvents(-1);
      return;
    }

    //  update dump
    //  cprintf("curr ptr is: %d | curr event is: %s\n", curr, syscalls_names[num]);
    events_syscall[curr] = syscalls_names[num]; //! memset here?
    events_cmd_name[curr] = proc->name;
    events_pid[curr] = proc->pid;
    curr++;
    if (curr == DUMPSIZE)
    {
      curr = 0;
      if (!full)
        full = 1;
    }

    //  trace
    if (num == SYS_exit && proc->trace && !proc->fflag)
    {
      if (!proc->eflag || !strncmp(proc->syscallName, "exit", strlen("exit")))
      {
        cprintf("TRACE: pid = %d | command name = %s | syscall = %s \n", proc->pid, proc->name, syscalls_names[num]);
      }
    }
    proc->tf->eax = syscalls[num]();
    // cprintf("process eflag:%d eflag name:%s | fflag %d sfflag %d \n", proc->eflag, proc->syscallName, proc->fflag, proc->sflag);
    events_return_value[curr] = proc->tf->eax;

    if (proc->sflag && proc->tf->eax == -1)
    {
      // print nothing
    }
    else if (proc->fflag && proc->tf->eax == 0)
    {
      // print nothing
    }
    else if (proc->eflag && strncmp(proc->syscallName, syscalls_names[num], strlen(proc->syscallName)))
    {
      // print nothing if syscall name does not match
    }
    else if (proc->trace)
    {
      cprintf("TRACE: pid = %d | command name = %s | syscall = %s | return value = %d\n", proc->pid, proc->name, syscalls_names[num], proc->tf->eax);
      // cprintf("process eflag:%d eflag name:%s | fflag %d sfflag %d \n", proc->eflag, proc->syscallName, proc->fflag, proc->sflag);
    }
  }
  else
  {
    cprintf("%d %s: unknown sys call %d\n", proc->pid, proc->name, num);
    proc->tf->eax = -1;
  }
}
