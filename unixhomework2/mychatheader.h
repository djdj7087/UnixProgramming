// mychatheader.h
// Created by wooyoung on 4/8/20.
//

#ifndef UNIXHOMEWORK2_MYCHATHEADER_H
#define UNIXHOMEWORK2_MYCHATHEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define EXIT_STATUS -1
#define SHM_RW_FLAG 0666

void chat_remove();

typedef struct message {
    long    messageID;          // message's id
    char    userID[20];         // user ID
    char    message[40];        // message string
} MESSAGE_INFO;

typedef struct chatInfo {
    MESSAGE_INFO    messageContainer[sizeof(MESSAGE_INFO)*100];     // for shared memory
    MESSAGE_INFO    quitMessage;                // message for quit
    int             totalMessageCount;          // number of messages in shared memory
} CHAT_INFO;

#endif //UNIXHOMEWORK2_MYCHATHEADER_H
