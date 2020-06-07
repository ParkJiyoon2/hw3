#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page.h"

// 페이지테이블 초기화
void pg_tbl_init()
{
	for(int i = 0; i < NPROC; i++)
	{
		for(int j = 0; j < PG_TBL_SIZE; j++)
		{	// 모두 -1 값으로 초기화. 
			pg_table[i][j] = -1;
		}
	}
}

// free list를 가르키는 포인터 
static struct frame *free_list;

// free_list가 가르키고있는 page_frame의 pfn을 리턴한다. 
int get_free_frame()
{
	return free_list->pfn;
}

// pfn initialize
void page_init()
{
	struct frame *p;
	struct frame *n;

	// initialize pg_frames
	memset(page_frame, 0, sizeof(page_frame));

	for(int i = 0; i < MAX_FRAMES; i++)
	{
		// 링크드 리스트로 연결  
		page_frame[i].pfn = i;

		if(i > 0)
			page_frame[i].p = &page_frame[i-1];

		if(i < MAX_FRAMES - 1)
			page_frame[i].n = &page_frame[i+1];
	}
	free_list = &page_frame[0];
}


////////////////////////

// 물리메모리 
int PMem[0x100000];

// 물리메모리 PMem[] 초기화 
void init_pmem()
{
	int i;
	for(i = 0; i < 0x100000; i++)
		PMem[i] = i;
}

// pa를 리턴
int access_pa(int pid, int va) 
{
	int pg_num = va / (16 * 16 * 16);
	int pg_offset = va % (16 * 16 * 16);


	if(pg_table[pid][pg_num] == -1) // mapping 안된상태라면
	{
		// mapping 하고
		pg_table[pid][pg_num] = get_free_frame();
		// free_list가 다음 page_frame을 가르키도록 함.
		free_list = (free_list->n);
	}
	
	//printf("***testing** pg_table[pid][pg_num] = %d\n", pg_table[pid][pg_num]);
	//printf("return value is : %d\n", pg_offset +( pg_table[pid][pg_num] *16*16*16));
	return pg_offset +( pg_table[pid][pg_num] *16*16*16);

}

int  main(int argc, char *argv[]) 
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

	printf("-----------------------start----------------\n");
	if (argc == 2) {
		input_file_name = argv[1];
	}
	fp = fopen(input_file_name, "r");

	pg_tbl_init();	// pg_table 초기화
	page_init();	// pfn 링크드리스트로 연결
	init_pmem();	// 물리메모리 초기화 


	// get a line from input
	while (getline(&line, &len, fp) != -1) {
		pid_str = strtok_r(line, " \n", &saveptr);
		va_str = strtok_r(NULL, " \n", &saveptr);

		pid = strtol(pid_str, &e, 16);
		va = strtol(va_str, &e, 16);
		
		// pa값을 받는다 
		pa = access_pa(pid, va);
		printf("pid: %d, va: 0x%08x pa: [0x%08x] = 0x%08X\n", pid, va, pa, PMem[pa]);

	}

	fclose(fp);
}
