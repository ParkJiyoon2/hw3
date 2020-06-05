
// virtual memory simulation
// gets input from input_vm
// prints output va -> pa

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page.h"

// assume Pmem 1MB
int PMem[0x100000];
static struct frame *free_list;

void init_pmem(){
	int i;
	for (i = 0 ; i < 0x100000 ; i++) {
		PMem[i] = i;
	}
}

void clear_pg_table(){ //pg 테이블 초기화
	for(int i=0; i<PG_TBL_SIZE; i++){
		pg_table[NPROC][i]=-1;
	}
}

void access_pa(int pid, int va, int *pa){ //물리주소 접근
	if(pg_table[NPROC][va>>12]==-1){ //비었으면
		free_list=get_free_frame();

		pg_table[NPROC][va>>12]=free_list->pfn;
		printf("fault! freepfn: 0x%x pgn: 0x%x ", free_list->pfn, va>>12);
		next_free_frame();
	}
	*pa=pg_table[NPROC][va>>12];
	*pa=*pa<<12;
	//printf("16진수 : %x ", va%4096);
	*pa=*pa+(va%4096);
}

void main(int argc, char *argv[]){

	char *input_file_name = "input_vm"; //파일 명
	FILE *fp; //input file ptr
	char *line = NULL;
	char *saveptr;

	char *pid_str;
	char *va_str;
	int pid; //프로세스 id
	int va; //기상 메모리 주소
	int pa; // 물리메모리 주소
	char *e;

	int pg;
	int pg_offset;
	size_t len;

	if (argc == 2) { //파일명 입력했으면
		input_file_name = argv[1];
	}
	fp = fopen(input_file_name, "r"); //파일 오픈

	page_init(); //페이지 생성
	init_pmem(); //초기화
	clear_pg_table(); //pg테이블 초기화

	// get a line from input
	while (getline(&line, &len, fp) != -1) {
		pid_str = strtok_r(line, " \n", &saveptr);
		va_str = strtok_r(NULL, " \n", &saveptr);

		pid = strtol(pid_str, &e, 16);
		va = strtol(va_str, &e, 16);
		pa = 0; // needs to be translated from va

		access_pa(pid, va, &pa);
		printf("pid: %d, va: 0x%08x pa: [0x%08x] = 0x%08X\n", pid, va, pa, PMem[pa]);
	}

	fclose(fp);
}
