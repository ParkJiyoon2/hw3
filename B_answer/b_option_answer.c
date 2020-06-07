#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page.h"

static struct frame *free_list;
// free frame list
struct frame * get_free_frame()
{
	return free_list;
}

struct frame * next_free_frame()
{
	free_list = free_list->n;
	return free_list;
}


void page_init()
{
	struct frame *p;
	struct frame *n;

	// initialize pg_frames
	memset(page_frame, 0, sizeof(page_frame));


	for (int i = 0 ; i < MAX_FRAMES ; i++) {
		// fill in free page frames list
		page_frame[i].pfn = i;
		if (i > 0)
			page_frame[i].p = &page_frame[i-1];

		if (i < MAX_FRAMES-1)
			page_frame[i].n = &page_frame[i+1];
	}
	free_list = &page_frame[0];
}


// assume Pmem 1MB * 0x100
int PMem[0x10000000];

void init_pmem()
{
	int i;
	for (i = 0 ; i < 0x10000000  ; i++) {
		PMem[i] = i;
	}
}


int pg_directory[NPROC][4] = {0}; //input_vm.2 에서는 L1이 4가지 밖에 안나옴. + pid는 10개임
int pg_directory_check[NPROC][4] = {0};


int access_pa(int pid, int va)
{
	int L1;
	int L2;
	int pg_offset;
	int pa;


	L1 = va/0x100000;
	L2 = va/0x1000%0x100;
        pg_offset = va%0x1000;

        // input_vm.2파일은 NPROC  == 10;


	 if(pg_directory_check[pid][L1] == 0)
         {
                free_list = get_free_frame();
                printf("fault! freepfn: 0x%x pdn: 0x%x  pid : 0x%x\n", free_list->pfn, L1, pid);
                pg_directory[pid][L1] = free_list->pfn*0x100000;
                next_free_frame();
                pg_directory_check[pid][L1] = 1;
         }

		pg_table[pid][L2] = pg_directory[pid][L1] + L2*0x1000;

		pa = pg_table[pid][L2] + pg_offset;

        return pa;
}



void main(int argc, char *argv[])
{
	char *input_file_name = "input_vm.2";
	FILE *fp; //input file ptr
	char *line = NULL;
	char *saveptr;

	char *pid_str;
	char *va_str;
	char *rw_str;

	int pid;
	int va;
	int rw;

	int pa;
	char *e;

	int pg;
	int pg_offset;
	size_t len;



	if (argc == 2) {
		input_file_name = argv[1];
	}
	fp = fopen(input_file_name, "r");

	page_init();
	init_pmem();

	printf("initial pg_table entries: 0x%x, pg size: 0x%x \n", PG_TBL_SIZE, PG_SIZE);

	// get a line from input
	while (getline(&line, &len, fp) != -1) {
		pid_str = strtok_r(line, " \n", &saveptr);
		va_str = strtok_r(NULL, " \n\t", &saveptr);
		rw_str = strtok_r(NULL, " \n", &saveptr);

		pid = strtol(pid_str, &e, 16);
		va = strtol(va_str, &e, 16);
		rw = strtol(rw_str, &e, 16);

		pa = access_pa(pid, va);

		printf("pid: %d, va: 0x%08x pa: [0x%08x] = 0x%08X\n", pid, va, pa, PMem[pa]);

	}

	fclose(fp);
}
