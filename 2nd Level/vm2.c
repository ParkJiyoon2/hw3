
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADDR_SIZE       0x1000000
#define PG_SIZE         0x1000
#define PG_TBL_SIZE     (ADDR_SIZE / PG_SIZE)
#define L1_SIZE         0x1000
#define L2_SIZE         0x100

int PMem[ADDR_SIZE];
int PMem1[L1_SIZE];
int PMem2[L2_SIZE];
int L1_Table[L1_SIZE];
int L2_Table[L2_SIZE];
int access_L1(int L1);
int access_L2(int L2);
void init_pmem();
void L1_init();
void L2_init();

void main(int argc, char *argv[])
{
        char *input_file_name = "input_vm.2";
        FILE *fp; //input file ptr
        char *line = NULL;
        char *saveptr;
        char *pid_str;
        char *va_str;
        char *io_str;
        int pnum = 0;
        int ion;
        int pid;
        int pa;
        int va;
        int pl1;
        int pl2;
        char *e;
        int pg;
        int l1;
        int l2;
        int pg_offset;
        size_t len;

        printf("initial pg_table entries: 0x%x, pg size: 0x%x\n", PG_TBL_SIZE, PG_SIZE);
        if (argc == 2) {
                input_file_name = argv[1];
        }
        fp = fopen(input_file_name, "r");

        L1_init();
        L2_init();
        init_pmem();

        // get a line from input
        while (getline(&line, &len, fp) != -1) {
                pid_str = strtok_r(line, " \n", &saveptr);
                va_str = strtok_r(NULL, " ", &saveptr);
                pid = strtol(pid_str, &e, 16);
                va = strtol(va_str, &e, 16);
                va = va % ADDR_SIZE;
                pg = va / PG_SIZE;
                l1 = pg / L2_SIZE;
                l2 = pg % L2_SIZE;
                pg_offset = va % PG_SIZE;
                pl1 = access_L1(l1);
                pl2 = access_L2(l2);
                pa = (pl1 << 20) + (pl2 << 12) + pg_offset;
                PMem[pa] = pnum;
                printf("pid: %d, va: 0x%08x L1: 0x%03x L2: 0x%02x pa: [0x%08x] = 0x%08X\n", pid, va, l1, l2, pa, PMem[pa]);
                pnum++;
        }

        fclose(fp);
}
void init_pmem(){

        for(int i = 0; i < ADDR_SIZE; i++){
                PMem[i] = i;
        }
}

int access_L1(int L1){

        for(int i = 0; i < L1_SIZE; i++){
                if(L1_Table[i] == L1){
                        return i;
                }
                else if(L1_Table[i] == -1){
                        L1_Table[i] = L1;
                        printf("fault! freeL1n: 0x%03x L1n: 0x%03x. ", i, L1);
                        return i;
                }
        }
}

int access_L2(int L2){

        for(int i = 0; i < L2_SIZE; i++){
                if(L2_Table[i] == L2){
                        return i;
                }
                else if(L2_Table[i] == -1){
                        L2_Table[i] = L2;
                        printf("fault! freeL2n: 0x%03x L2n: 0x%03x. ", i, L2);
                        return i;
                }
        }
}

void L1_init(){
        for(int j = 0; j < L1_SIZE; j++){
                L1_Table[j] = -1;
        }
}

void L2_init(){
        for(int k = 0; k < L2_SIZE; k++){
                L2_Table[k] = -1;
        }
}
