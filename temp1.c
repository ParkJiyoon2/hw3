#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	FILE *rfile;
	rfile = fopen((char *) argv[1], "r");

	if(rfile == NULL)
	{
		perror("no rfile");
		exit(0);
	}

	ssize_t read = 0;
	size_t len = 0;
	char *line = NULL;

	read = getdelim(&line, &len, '\n', rfile);

	printf("%s \n", line);
}
