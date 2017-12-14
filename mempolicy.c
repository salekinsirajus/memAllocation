#include "pm.h"
#include <minix/callnr.h>
#include <minix/com.h>
#include <signal.h>
#include "mproc.h"
#inlcude "memheader.c"

#define FIRST_FIT 0
#define NEXT_FIT 1
#define BEST_FIT 2
#define WORST_FIT 3
#define RANDOM_FIT 4

int CUSTOM_MEM_POLICY = FIRST_FIT;

int do_cyclemempolicy(void){
    CUSTOM_MEM_POLICY = (CUSTOM_MEM_POLICY + 1) % 5;

    return(OK);
}
