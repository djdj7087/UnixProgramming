#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LIMIT 20

int docommand(char* command)
{
	pid_t pid;

	printf("%s", command);

	if((pid = fork()) < 0)
			return -1;
	if(pid == 0) { /* Child Process */
		execl("/bin/sh", "sh", "-c", command, NULL);
		printf("\n");
		perror("execl: ");
		exit(1);
	}

	// Parent Process
	// Wait Until Child Process Exits

	wait(NULL);
	return 0;
}

int main()
{
	/*
	   // Do not use gets()
	char str[20];

	fgets(str);
	*/

	char str[MAX_LIMIT];
	
	perror("Error Test!!");
	for (;;) {
		printf("Press ctrl+c to get out of myShell!!\n\n");

		printf("[myShell]$ ");
		fgets(str, MAX_LIMIT, stdin);
		docommand(str);
	}

	return 0;
}
