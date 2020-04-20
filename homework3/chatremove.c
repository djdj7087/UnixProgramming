/*
 * chatremove.c -- CP33357 assignment 3, Spring 2020
 * Choi Woo Young, id #201524600, section 059, TAK. SUNG WOO
 */

#include "mychatheader.h"

void chatremove() {
    int shm_id;

    shm_id = shmget((key_t) 201524600, sizeof(CHAT_INFO), SHM_RW_FLAG);

    if(shm_id < 0) {
        printf("Cannot found Shared Memory : id [%d]\n", shm_id);
        perror("shmget failed: ");
        exit(EXIT_STATUS);
    }

    if(shmctl(shm_id, IPC_RMID, 0) < 0) {
        printf("Failed to delete shared memory\n");
        exit(EXIT_STATUS);
    } else {
        printf("Successfully delete shared memory\n");
    }
}