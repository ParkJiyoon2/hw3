#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "page.h"
#include <semaphore.h>
#include <string.h>

int PMem[0x1000000]; //주소 공간 제한
static struct frame *free_list;


void init_pmem(){
	for(int i=0; i<0x1000000; i++){
		PMem[i]=i;
	}
}

void init_pg_table(){
	for(int i=0; i<NPROC; i++){
		for(long o=0; o<PG_TBL_SIZE; o++){
			pg_table[i][o]=-1;
		}
	}
}

long access_pa(int pid, int va, FILE *ofp){
	
	if(pg_table[pid][va>>12]==-1){ //MAPPING이 안되어 있으면 demand paging 실시
		free_list=get_free_frame(); //해당 노드를 가져옴	
		pg_table[pid][va>>12]=free_list->pfn;
		
		printf("fault! freepfn: 0x%x pgn: 0x%x ", free_list->pfn, va>>12);
		
		next_free_frame(); //다음 노드를 가리킴
		
	}
	long pa=pg_table[pid][va>>12];
	
	pa=pa<<12;
	
	pa=pa+(va%4096);

	return pa;
}

int main(int argc, char *argv[])
{
	pid_t pid;//프로세스
	int sem;//세마포어 정상인지 확인하는 변수
	char *input_file_name="input_vm.2"; //실행하는 파일명

	FILE *fp; //input file ptr
	char *line=NULL; //파일 내용
	char *saveptr;

	char *pid_str;
	char *va_str;
	char *e;
	int process_id, va;
	int pa;
	size_t len;

	FILE* ofp=fopen("output_vm.text", "w");

	if(argc==2){ //매개변수 확인
		input_file_name = argv[1];
	}

	fp=fopen(input_file_name, "r"); //파일 오픈
	
	page_init(); //페이지 테이블 초기화
	init_pmem(); //물리 메모리 초기화
	init_pg_table(); //페이지 테이블 초기화

	while(getline(&line, &len, fp)!=-1){ //내용 읽기
		pid_str=strtok_r(line, " \n", &saveptr);
		va_str=strtok_r(NULL, " \n", &saveptr);

		process_id=strtol(pid_str, &e, 16); //프로세스 ID
		
		va=strtol(va_str, &e, 16); //가상 메모리 주소
		pa=access_pa(process_id, va, ofp); //물리 메모리 주소
		
		printf("pid: %d, va: 0x%08x pa: [0x%08x] = 0x%08x\n", process_id, va, pa, PMem[pa]);
	}
	fclose(fp);
	

	return 0;
}

