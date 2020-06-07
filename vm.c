#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page.h"


int PMem[0x100000];
//static struct frame *free_list;


void init_pmem()
{
	int i;
	for (i = 0; i < 0x100000; i++) {
		PMem[i] = i;
	}
}


void access_pa(int pid, int va) {
	int pa;
	if (pg_table[NPROC][va >> 12] == -1) {		
		pg_table[NPROC][va >> 12] = get_free_frame() -> p -> pfn;		
		printf("^11234^fault! freepfn: 0x%x pgn: 0x%x ", pg_table[NPROC][va >> 12], va >> 12);
												
	}
	pa = (pg_table[NPROC][va >> 12]<<12) + (va & 0x00000fff);	
	printf("8pid: %d, va: 0x%08x pa: [0x%08x] = 0x%08X\n", pid, va, pa, PMem[pa]);
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
	
	size_t len;		

	if (argc == 2) {		
		input_file_name = argv[1];		
	}
	fp = fopen(input_file_name, "r");	

	page_init();		
	init_pmem();		

	// init page table
	//pg_table[NPROC][PG_TBL_SIZE] 
	for(int i = 0; i<PG_TBL_SIZE; i++){
	memset(pg_table[i], -1, sizeof(NPROC) * PG_TBL_SIZE );
	}
	
	while (getline(&line, &len, fp) != -1) {
		pid = strtol(strtok_r(line, " \n", &saveptr), &e, 16);	
		va = strtol(strtok_r(NULL, " \n", &saveptr), &e, 16);	
		access_pa(pid, va);	
	}

	fclose(fp);
}


