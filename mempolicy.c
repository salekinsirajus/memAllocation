#include "pm.h"
#include <minix/callnr.h>
#include <minix/com.h>
#include <signal.h>
#include "mproc.h"
#include "memheader.c"

int do_cyclemempolicy(void){
    CUSTOM_MEM_POLICY = (CUSTOM_MEM_POLICY + 1) % 5;

    return(OK);
}
