#include <stdio.h>
#include "/usr/src/minix/servers/is/inc.h"
#include <minix/sysinfo.h>
#include <minix/com.h>
#include <lib.h>
#include "/usr/src/minix/servers/pm/mproc.h"

int main(){
    struct pm_mem_info pmi;
    int i;

    getsysinfo(PM_PROC_NR, SI_MEM_ALLOC, &pmi);
    for (i = 0; i < _NR_HOLES; i++){
        printf("Hole size: %d", pmi.pmi_holes[i].h_len);
        }

return 0;
}
