#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "chatshm.h"

int main()
{
	int 		shmid;
	CHAT_INFO 	*chatInfo = NULL;

	void *shmaddr = (void*) 0;

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

	chatInfo = (CHAT_INFO *)shmaddr;
	
	printf("----------------- Chat Message ----------------\n");
	while(1) {
		printf("[%s]%ld: %s\n",
				chatInfo->userID,
				chatInfo->messageTime,
				chatInfo->message);

		if (!strcmp(chatInfo->message, "/exit\n")) {
			printf("%s is out\n", chatInfo->userID);
			break;
		}

		sleep(3);
	}
	return 0;
}
