//
// Created by wooyoung on 4/23/20.
//

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t tid[2];
int counter;

void *doSomething(void *arg) {
    unsigned long i = 0;

    counter += 1;

    printf("Job %d started\n", counter);

    /*
     * TODO Insert sleep or some long dummy codes to delay thread
     *  It is takes long time, times up and the other thread will run
     */
    for(i = 0 ; i < (0xFFFFFFFF) ; i++);

    printf("Job %d finished\n", counter);

    return NULL;
}

int main(void) {
    int i = 0;
    int err;

    while (i < 2) {
        err = pthread_create(&tid[i], NULL, doSomething, NULL);
        if (err != 0) {
            perror("thread creation failed: \n");
            exit(EXIT_FAILURE);
        }
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    return 0;
}