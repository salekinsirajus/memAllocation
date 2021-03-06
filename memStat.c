#include <unistd.h>
#include "/usr/src/servers/is/inc.h"
#include "/usr/src/servers/pm/mproc.h"

int main(int argc, char * argv[]) {
  struct pm_mem_info pmi;
  int i;

  getsysinfo(PM_PROC_NNR, SI_MEM_ALLOC, &pmi);
  
  for(i = 0; i < _NR_HOLES; i++) {
    printf("Hole size: %d", pmi.pmi_holes[i].h_len);
  }
  return 0;
}
