// mychatheader.h -- CP33357 assignment 2, Spring 2020
// Choi Woo Young, id #201524600, section 059, TAK. SUNG WOO

#ifndef UNIXHOMEWORK2_MYCHATHEADER_H
#define UNIXHOMEWORK2_MYCHATHEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define EXIT_STATUS -1          // Exit status
#define SHM_RW_FLAG 0666        // Shared memory Read Write Flag

void chat_remove();

/* structure for message... */
typedef struct message {
    long    messageID;          // message's id
    char    userID[20];         // user ID
    char    message[40];        // message string
} MESSAGE_INFO;

/* structure for shared memory... */
typedef struct chatInfo {
    MESSAGE_INFO    messageContainer[sizeof(MESSAGE_INFO)*100];     // for shared memory
    MESSAGE_INFO    quitMessage;                // message for quit
    int             totalMessageCount;          // number of messages in shared memory
} CHAT_INFO;

#endif //UNIXHOMEWORK2_MYCHATHEADER_H
