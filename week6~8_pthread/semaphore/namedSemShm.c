//
// Created by wooyoung on 4/27/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>

#define LOOP 100

/*
 * Global Variables
 * sem_t *semP;
 * sem_t *semC;
 */

int shmRemove(int shm_id, int *data) {
    /*---- Delete shared memory ----*/
    shmdt(data);

    if (shmctl(shm_id, IPC_RMID, 0) < 0) {
        printf("Failed to delete shared memory\n");
        return -1;
    } else {
        printf("Successfully delete shared memory\n");
    }
}

int main() {
    // fork
    pid_t child_pid;

    // shared memory
    key_t key;
    int shm_id;
    int *data;
    int prev = 2020;

    /*---- semaphore ----*/
    // Heap
    sem_t *semP = (sem_t *) malloc(sizeof(sem_t));
    sem_t *semC = (sem_t *) malloc(sizeof(sem_t));

    /* Stack
     * sem_t *semP;
     * sem_t *semC;
     */

    int pShared = 1;
    unsigned int valueP = 1;
    unsigned int valueC = 0;

    key = ftok("namedSemShm.c", 'R');
    shm_id = shmget(key, sizeof(int), 0644 | IPC_CREAT | IPC_EXCL);

    /*---- If shm have already existed..... just get shm ----*/
    if (shm_id < 0) {
        shm_id = shmget(key, sizeof(int), 0644);
        data = shmat(shm_id, (void *)0, 0644);
        if (data < (int *)0) {
            perror("shmat attach is failed: ");
            exit(EXIT_FAILURE);
        }
    } else {
        data = shmat(shm_id, (void *) 0, 0644);
    }

    /*---- data in prev (2020) ----*/
    *data = prev;

    /*---- unlink semaphore before open semaphore ----*/
    sem_unlink("pSem");
    sem_unlink("cSem");

    /*---- named Semaphore ----*/
    semP = sem_open("pSem", O_CREAT | O_EXCL, 0644, valueP);

    if (semP == SEM_FAILED) {
        shmRemove(shm_id, data);
        perror("sem_open [pSem] : ");
        exit(EXIT_FAILURE);
    }

    semC = sem_open("cSem", O_CREAT | O_EXCL, 0644, valueC);

    if (semC == SEM_FAILED) {
        shmRemove(shm_id, data);
        perror("sem_open [cSem] : ");
        exit(EXIT_FAILURE);
    }

    /*---- fork ----*/
    if ((child_pid = fork()) < 0) {
        perror("Fork Failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) { // Child Process
        for (int i = 0; i < LOOP; i++) {
            sem_wait(semP);
            *data = *data + 1;
            printf("Child data: %d\n", *data);
            sleep(2);
            sem_post(semC);
        }
    } else { // Parent Process
        for (int i = 0; i < LOOP; i++) {
            sem_wait(semC);
            *data = *data - 1;
            printf("Parent data: %d\n", *data);
            sem_post(semP);
        }
    }

    /*---- delete Semaphore and shared memory ----*/
    shmRemove(shm_id, data);
    sem_unlink("pSem");
    sem_unlink("cSem");

    return 0;
}