// chat.c -- CP33357 assignment 2, Spring 2020
// Choi Woo Young, id #201524600, section 059, TAK. SUNG WOO

#include "mychatheader.h"

int main(int argc, char *argv[]) {
    key_t       shm_key = 20200406;         // Key for Shared Memory
    int         shm_id;                     // shared memory's ID
    int         start_index = 0;            // start index for message
    char        userID[20];                 // Variable for User ID
    CHAT_INFO   *chatInfo = NULL;           // Structure for Chatting

    void *shmaddr;

    /* If user id is not exist, print error message and exit program */
    if (argc < 2) {
        fprintf(stderr, "[Usage]: ./shmwrite UserID \n");
        exit(EXIT_STATUS);
    }

    /* String Copy from Argument for user ID */
    strcpy(userID, argv[1]);

    /* Create Shared memory */
    shm_id = shmget(shm_key, sizeof(CHAT_INFO),
                    SHM_RW_FLAG | IPC_CREAT | IPC_EXCL);

    /* If shm have already existed...... just get shm */
    if (shm_id < 0) {
        shm_id = shmget(shm_key, sizeof(CHAT_INFO), SHM_RW_FLAG);
        shmaddr = shmat(shm_id, (void *) 0, SHM_RW_FLAG);

        /* If shm attach is failed... */
        if (shmaddr < (void *) 0) {
            perror("shm attach is failed: ");
            exit(EXIT_STATUS);
        }
    } else {
        /* Attach shared memory*/
        shmaddr = shmat(shm_id, (void *) 0, SHM_RW_FLAG);
    }

    chatInfo = (CHAT_INFO *) shmaddr;

    while (1) {
        char message[40];

        /* shared memory write */
        printf("---------------Enter Message---------------\n");
        printf("[%s] >> ", userID);
        fgets(message, 40, stdin);
        if (!strcmp(message, "..\n")) {
            printf("---------------Unread Messages---------------\n");
            /*
             * shared memory read from start_index to last message
             */
            for (int i = start_index; i < chatInfo->totalMessageCount; i++) {
                printf("%ld/[%s] >> %s",
                       chatInfo->messageContainer[i].messageID,
                       chatInfo->messageContainer[i].userID,
                       chatInfo->messageContainer[i].message);
            }
            start_index = chatInfo->totalMessageCount;

        } else {
            /* If message was "/bye" then, quit message stored */
            if (!strcmp(message, "/bye\n")) {
                strcpy(chatInfo->quitMessage.userID, userID);
                chatInfo->quitMessage.messageID = -1;
                strcpy(chatInfo->quitMessage.message, message);
            }
            /* When normal message is coming... */
            else {
                strcpy(chatInfo->messageContainer[chatInfo->totalMessageCount].userID,
                        userID);
                chatInfo->messageContainer[chatInfo->totalMessageCount].messageID =
                        chatInfo->totalMessageCount;
                strcpy(chatInfo->messageContainer[chatInfo->totalMessageCount].message,
                        message);
                chatInfo->totalMessageCount++;
            }
        }

        if (!strcmp(chatInfo->quitMessage.message, "/bye\n")) {
            printf("%s is out\n",
                   chatInfo->quitMessage.userID);
            chat_remove();
            break;
        }

    }

    return 0;
}