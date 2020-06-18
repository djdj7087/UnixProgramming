//
// Created by wooyoung on 4/23/20.
//

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t tid[2];
pthread_mutex_t lock;
int counter;

void *doSomething(void *arg) {
    pthread_mutex_lock(&lock);

    counter += 1;

    printf("Job %d started\n", counter);

    /*
     * TODO Insert sleep or some long dummy codes to delay week6_thread
     *  It is takes long time, times up and the other week6_thread will run
     */
    for(unsigned long i = 0 ; i < (0xFFFFFFFF) ; i++);

    printf("Job %d finished\n", counter);

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main(void) {
    int i = 0;
    int err;

    while (i < 2) {
        err = pthread_create(&tid[i], NULL, doSomething, NULL);
        if (err != 0) {
            perror("week6_thread creation failed: \n");
            exit(EXIT_FAILURE);
        }
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    return 0;
}