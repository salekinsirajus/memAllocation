#define FIRST_FIT 0
#define NEXT_FIT 1
#define BEST_FIT 2
#define WORST_FIT 3
#define RANDOM_FIT 4

int CUSTOM_MEM_POLICY = FIRST_FIT;

void do_cyclemempolicy(void){
    CUSTOM_MEM_POLICY = (CUSTOM_MEM_POLICY + 1) % 5;
    printf("mem policy is %d \n", CUSTOM_MEM_POLICY);

    return;
}
