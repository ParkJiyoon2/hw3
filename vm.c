
// virtual memory simulation
// gets input from input_vm
// prints output va -> pa

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page.h"

// assume Pmem 1MB
int PMem[0x10000];
int Map[PG_TBL_SIZE];

void init_pmem()
{
        int i;
        for (i = 0 ; i < 0x10000 ; i++) {
                PMem[i] = i;
        }
}

int access_pa(int pid, int va)
{
        for(int i = 0; i < sizeof(Map); i++){
                if(Map[i] == -1){
                        Map[i] = va;
                        printf("fault! freepfn: 0x%x pgn: 0x%x ", i, va);
                        return i;
                }
                else if(Map[i] == va){
                        return i;
                }
        }
}

void main(int argc, char *argv[])
{
        char *input_file_name = "input_vm";;
        FILE *fp; //input file ptr
        char *line = NULL;
        char *saveptr;

        char *pid_str;
        char *va_str;
        int pid;
        int va;
        int pa;
        char *e;

        int pg;
        int pg_offset;
        size_t len;

        printf("initial pg_table entries: 0x%x, pg size: 0x%x\n", PG_TBL_SIZE, PG_SIZE);
        if (argc == 2) {
                input_file_name = argv[1];
        }
        fp = fopen(input_file_name, "r");

        page_init();
        init_pmem();

        // get a line from input
        while (getline(&line, &len, fp) != -1) {
                pid_str = strtok_r(line, " \n", &saveptr);
                va_str = strtok_r(NULL, " \n", &saveptr);
                pid = strtol(pid_str, &e, 16);
                va = strtol(va_str, &e, 16);
                pg = va >> 12;
                pg_offset = va % 0x1000;
                pa = access_pa(pid, pg);
                pa = (pa << 12) + pg_offset;
                printf("pid: %d, va: 0x%08x pa: [0x%08x] = 0x%08X\n", pid, va, pa, PMem[pa]);
        }

        fclose(fp);
}

void page_init(){
        for(int i = 0; i < sizeof(Map); i++){
                Map[i] = -1;
        }
}
