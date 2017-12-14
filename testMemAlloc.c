#include <stdio.h>
#include <lib.h>
#include <sys/syscall.h>

int main(){
  message m;
  printf("Calling PM_CYCLEMEMPLOICY using system calls\n");
  _syscall(PM_PROC_NR, 48, &m);
return 0;
}
