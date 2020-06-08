
// virtual memory simulation
// gets input from input_vm
// prints output va -> pa

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

//페이지 초기화
void page_init()
{
	struct frame *p;
	struct frame *n;

	// initialize pg_frames
	memset(page_frame, 0, sizeof(page_frame));

	for (int i = 0; i < MAX_FRAMES; i++) {
		// fill in free page frames list
		page_frame[i].pfn = i;
		if (i > 0)
			page_frame[i].p = &page_frame[i - 1];

		if (i < MAX_FRAMES - 1)
			page_frame[i].n = &page_frame[i + 1];
	}
	free_list = &page_frame[0];
}

// assume Pmem 1MB
int PMem[0x100000];

//물리메모리 초기화
void init_pmem()
{
	int i;
	for (i = 0; i < 0x100000; i++) {
		PMem[i] = i;
	}
}

//페이지 테이블 초기화
void init_page_table() {

	int i;
	for (i = 0; i < PG_TBL_SIZE; i++) {
		pg_table[NPROC][i] = -1;
	}
}

//물리 메모리 주소
int access_pa(int pid, int va)
{
	int pg_num = va >> 12; //Page number

	if (pg_table[NPROC][pg_num] == -1) { //테이블이 비었으면

		free_list = get_free_frame();
		pg_table[NPROC][pg_num] = free_list->pfn;
		printf("fault! freepfn: 0x%x pgn: 0x%x ", pg_table[NPROC][pg_num], pg_num);
		free_list = free_list->n; //다음 프레임 가리킴

	}

}


void main(int argc, char *argv[])
{
	char *input_file_name = "input_vm";
	FILE *fp; //input file ptr
	char *line = NULL;
	char *saveptr;

	char *pid_str;
	char *va_str;
	int pid; // process id
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
	
	
	printf("initial pg_table entries: 0x%x, pg size: 0x%x \n", PG_TBL_SIZE, PG_SIZE);

	page_init();
	init_pmem();
	init_page_table();


	// get a line from input
	while (getline(&line, &len, fp) != -1) {
		pid_str = strtok_r(line, " \n", &saveptr);
		va_str = strtok_r(NULL, " \n", &saveptr);

		pid = strtol(pid_str, &e, 16);
		va = strtol(va_str, &e, 16);
		pa = 0; // needs to be translated from va
		access_pa(pid, va);

		int pg_num = va >> 12;
		int pg_offset = 0x00000fff & va;
		pa = (pg_table[NPROC][pg_num] << 12) + (pg_offset);

		printf("pid: %d, va: 0x%08x pa: [0x%08x] = 0x%08X\n", pid, va, pa, PMem[pa]);
	}
}

