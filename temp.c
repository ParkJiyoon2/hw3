#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
	FILE *fp = fopen("input_vm.2", "r");
	char *line = NULL;
	char *pid_s=NULL;
	char *va_s=NULL;
	char *saveptr;
	size_t len;
	while(getline(&line, &len, fp) != -1){
		getline(&line, &len, fp);
		pid_s = strtok_r(line, " \n", &saveptr);
		va_s = strtok_r(NULL, " \n", &saveptr);
		if(va_s[3]!='0'){
			printf("aa \n");
		}
	}
	free(line);
	fclose(fp);
	return 0; 
}
