#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "page.h"
#include <semaphore.h>

int PMem[0x100000];


void init_pmem(){
	int i;
	for(i=0; i<0x100000; i++){
		PMem[i]=i;
	}
}

int access_pa(int pid, int va){


}

int main(int argc, char *argv[])
{
	pid_t pid;

	char *input[]={"pwd", 0};
	sem_t mutex;
	
	int sem;

	if(sem=sem_init(&mutex,1,1)<0){//세마포어 생성
		perror("semaphore initilizaion");
		exit(0);
	}
	
	printf("sem init : %d\n", sem);

	page_init();
	init_pmem();
	
	for (int i = 0 ; i < 10 ; i++) {
		
		pid = fork();
		if (pid == -1) {
			perror("fork error");
			return 0;
		}
		else if (pid == 0) {
			// child`
			printf("child process with pid %d (i: %d) \n", getpid(), i);
			
			
			sem_wait(&mutex);		
			printf("%d : %d\n", getpid(), i);
			sem_post(&mutex);
			
			
			exit (0);
		} else {
			// parent
			printf("parent preocess waiting\n");
			wait(0);
		}
		
	}

	return 0;
}

