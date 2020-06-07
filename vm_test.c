
// virtual memory simulation
// gets input from input_vm
// prints output va -> pa

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page.h"

// assume Pmem 1MB
int PMem[0x100000];
int INDEX = 0;
int ALPHA = 87;
int INTGR = 48;
int on_demand[16] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
void init_pmem()
{
	int i;
	for (i = 0 ; i < 0x100000 ; i++) {
		PMem[i] = i;
	}
}

int get_frame(int par, int *ptr, int *pfn){
	if(on_demand[par] != -1){
		*ptr = 1;
		return on_demand[par];
	} 
	on_demand[par] = INDEX;
	*pfn = INDEX;

	INDEX++;
	*ptr = 0;
	return on_demand[par];
}

int access_pa(int pid, int va, int *ptr, int *pfn)
{
	char tmp[5];
	char ch;
	int cvted;
	int val;
	char hex[2];
	char resp[5] = "";
	sprintf(tmp, "%x", va);
      	if(strlen(tmp)==4){
		ch = tmp[0];
	} else {
		ch = '0';
	}
	if(ch >= 97){
		val = get_frame(ch-ALPHA, ptr, pfn);
	} else {
		val = get_frame(ch-INTGR, ptr, pfn);
	}
	sprintf(hex, "%x", val);
	strcat(resp, hex);
	if(ch == '0'){
		strcat(resp, tmp);
	} else {
		strcat(resp, &tmp[1]);
	}
	cvted = strtol(resp, NULL, 16);
	return cvted;
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

	int freepfn = 0;
	int flag = 0;
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

		pid = strtol(pid_str, NULL, 16);
		va = strtol(va_str, NULL, 16);
		pa = access_pa(pid, va, &flag, &freepfn);
		//pa = 0;
		if(flag == 0) {
			printf("Page Fault!! freepfn: 0x%x ", freepfn);
		}
		printf("pid: %d, va: 0x%08x pa: [0x%08x] = 0x%08X\n", pid, va, pa, PMem[pa]);
	}
	free(line);
	fclose(fp);
}
