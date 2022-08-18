#include "types.h"
#include "x86.h"
#include "defs.h"
// #include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{

  int status = 0;
  if(argint(0, &status) < 0)
    return -1;
  return exit(status);
  // return 0;  // not reached
}

int
sys_wait(void)
{
   int *p;

  if(argptr(0, (void*)&p, sizeof(int))<0){

    return -1;

  }


  return wait(p);
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;

  //added on 4 aug ,to increase memory size by adding the amount of size the memeory is grown or decreased;
  myproc()->currentSize += n;

  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
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
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_helloWorld(void){

return helloWorld();

}

int
sys_numOpenFiles(void){

return numOpenFiles();

}

int sys_memAlloc(void){

  return memAlloc();
}

int sys_getprocesstimedetails(void){

  return getprocesstimedetails();
}

int sys_currentlyRunning(void){
  return currentlyRunning();
}

int sys_processInfo(void){
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  // cprintf("pid from sysproc.c %d\n" , pid);
  return processInfo(pid);
}

// int sys_myshell(void){
//   return myshell();
// }