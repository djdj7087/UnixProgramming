//
// Created by wooyoung on 4/23/20.
//

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

sem_t semp;
int val;

void *wait_fun(void *arg) {
    printf("wait_fun thread starts...\n");

    while (1) {
        sem_wait(&semp);
        sem_getvalue(&semp, &val);

        printf("wait_fun thread: sem_value: %d\n", val);
    }
}

int main(void) {

    pthread_t pthread_tid = 0;

    if (sem_init(&semp, 0, 1)) {
        perror("sem_init error: \n");
        exit(EXIT_FAILURE);
    }

    sem_post(&semp);
    sem_post(&semp);
    sem_post(&semp);
    sem_post(&semp);

    sem_getvalue(&semp, &val);

    printf("main: sem_value: %d\n", val);

    if (pthread_create(&pthread_tid, NULL, wait_fun, NULL) != 0) {
        perror("Thread create failed: \n");
        exit(EXIT_FAILURE);
    }

    pthread_join(pthread_tid, NULL);

    exit(EXIT_FAILURE);
}