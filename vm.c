// virtual memory simulation
// gets input from input_vm
// prints output va -> pa

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page.h"

// assume Pmem 1MB
// PMem = 4byte 짜리로 1MB 개만듬.
int PMem[0x100000];

//PMem의 i인덱스 값을 i로 초기화 해주는 함수
void init_pmem()
{
	int i;
	for (i = 0 ; i < 0x100000 ; i++) {
		PMem[i] = i;// PMem[0]=0, PMem[4]=4
	}
}

//얘는 뭐하는 놈이지?
int access_pa(int pid, int va)
{
	int pa=0;
	int l1 = va * (0x11110000);//table index
	int offset = va * (0x111);

	if(pg_table[0][l1] = -1 )//table에 index가 존재하는지?
		printf("Demand paging 요구가 발생하였습니다.");
		pg_table[0][l1] = get_free_frame()->pfn; // Demand paging
	pa = l1+offset;
	return pa;
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

	//pglist.c에 구현되있음
	page_init();
	init_pmem();

	// get a line from input
	while (getline(&line, &len, fp) != -1) {//한줄씩 읽는다.
		pid_str = strtok_r(line, " \n", &saveptr);
		va_str = strtok_r(NULL, " \n", &saveptr);

		pid = strtol(pid_str, &e, 16);
		va = strtol(va_str, &e, 16);
		//여기가 중요!
		pa = access_pa(pid,va);


		printf("pid: %d, va: 0x%08x pa: [0x%08x] = 0x%08X\n", pid, va, pa, PMem[pa]);
	}

	fclose(fp);
}
