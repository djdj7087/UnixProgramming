#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "chatshm.h"

int main(int argc, char* argv[])
{
	int 		shmid;
	char		userID[20];
	CHAT_INFO 	*chatInfo = NULL;

	void *shmaddr = (void*) 0;

	if (argc < 2) {
		fprintf(stderr, "[Usage]: ./shmwrite UserID \n");
		exit(-1);
	}
	
	strcpy(userID, argv[1]);
	
	shmid = shmget((key_t)7087, sizeof(CHAT_INFO), 0666 |
					IPC_CREAT | IPC_EXCL);

	if (shmid < 0) {

		// If shm have already existed...... just get shm
		shmid = shmget((key_t)7087, sizeof(CHAT_INFO), 0666);
		shmaddr = shmat(shmid, (void *)0, 0666);
		if (shmaddr < (void *)0) {
			perror("shmat attach is failed: ");
			exit(-1);
		}

	}

	else {
		shmaddr = shmat(shmid, (void *)0, 0666);
	}
	
	chatInfo = (CHAT_INFO *)shmaddr;
	
	printf("----------------- Chat Message ----------------\n");
	while(1) {
		char inputstr[40];

		printf("Enter your message: ");
		fgets(inputstr, 40, stdin);
		strcpy(chatInfo->userID, userID);
		chatInfo->messageTime++;
		strcpy(chatInfo->message, inputstr);

	}
	return 0;
}
