
// virtual memory simulation
// gets input from input_vm
// prints output va -> pa

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page_HW3.h"

// assume Pmem 1MB
int PMem[0x100000]; //물리 메모리 = 1MB (0~255 의 pageframenumber)
static struct frame *free_list;

void init_pmem() //pmem 초기화
{
	int i;
	for (i = 0 ; i < 0x100000 ; i++) {
		PMem[i] = i;
	}
}

void init_pg_table(){ // page table 초기화
	for(int i=0; i<PG_TBL_SIZE; i++)

		pg_table[NPROC][i]=-1;

}

int access_pa(int pid, int va) //물리주소로 접근해서 매핑해준다.
{
	int page_number = (0xfffff000 & va) >> 12; //뒤에서 4번째 자리가 page_number
    int page_offset = 0x00000fff & va; // 뒤의 3자리가 page_offset


	if(pg_table[NPROC][page_number] == -1){ // page table이 비어 있거나 invalid이면

		free_list = get_free_frame();
		pg_table[NPROC][page_number] = free_list->p->pfn;

		printf("fault! freepfn: 0x%x pgn: 0x%x ", free_list->pfn, page_number);
	}

	return (pg_table[NPROC][page_number]<<12) + page_offset;
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

	if (argc == 2) {
		input_file_name = argv[1];
	}
	fp = fopen(input_file_name, "r");

	page_init(); //page_frame 초기화
	init_pmem(); //pmem 초기화
	init_pg_table(); //page table 초기화

	// get a line from input
	while (getline(&line, &len, fp) != -1) {
		pid_str = strtok_r(line, " \n", &saveptr);
		va_str = strtok_r(NULL, " \n", &saveptr);

		pid = strtol(pid_str, &e, 16);
		va = strtol(va_str, &e, 16);
		pa = access_pa(pid, va); //매핑

		printf("pid: %d, va: 0x%08x pa: [0x%08x] = 0x%08X\n", pid, va, pa, PMem[pa]);
	}

	fclose(fp);
}
