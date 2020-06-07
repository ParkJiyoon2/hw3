#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page.h"

// assume Pmem 1MB
int PMem[0x100000];
static struct frame *free_list;

struct frame get_free_frame(){
  free_list = free_list ->n;
  return free_list;
}

void init_pmem(){
	int i;
	for (i = 0 ; i < 0x100000 ; i++) {
		PMem[i] = i;
	}
}

void init_pg_table(){
	for(int i=0; i<PG_TBL_SIZE; i++){
		pg_table[NPROC][i]=-1;
	}
}

int access_pa(int pid, int va){  //PA접근
	int pg_num;  // 오른쪽으로 12bit shift하여 offset값을 버려줌(?) 떼어냄
  int pg_offset; // 뒤 12bit가 offset

  pg_num = va >> 12;
  pg_offset = (va & 0x00000fff);
  // 0xoooooooo 에서 f는 그 부분을 떼어오는 의미라고 쉽게생각

  if(pg_table[NPROC][pg_num] === -1){  // pg_table이 비어있다면
    free_list = get_free_frame();
    pg_table[NPROC][pg_num] = free_list -> p -> pfn;
    printf("fault! freepfn: 0x%x pgn: 0x%x ", free_list->pfn, pg_num)
  }
  return (pg_table[NPROC][pg_num] << 12) + pg_offset;
}

void main(int argc, char *argv[]){

	char *input_file_name = "input_vm"; //파일 명
	FILE *fp;
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

	page_init();
	init_pmem();
	init_pg_table();

	// get a line from input
	while (getline(&line, &len, fp) != -1) {
		pid_str = strtok_r(line, " \n", &saveptr);
		va_str = strtok_r(NULL, " \n", &saveptr);

		pid = strtol(pid_str, &e, 16);
		va = strtol(va_str, &e, 16);
		pa = access_pa(pid, va);


		printf("pid: %d, va: 0x%08x pa: [0x%08x] = 0x%08X\n", pid, va, pa, PMem[pa]);
	}

	fclose(fp);
}
