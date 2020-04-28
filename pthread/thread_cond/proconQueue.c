//
// Created by wooyoung on 4/26/20.
//
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "proconQueueHeader.h"

/* Global is better than Local */
int ret_count;

queue *queueInit(void) {
    queue *q;

    // TODO Create an initial queue
    q = (queue *) malloc(sizeof(queue));
    if (q == NULL)
        return NULL;

    q->empty = 1;       // 1 means Yes or True
    q->full = 0;        // 0 means No or False
    q->head = 0;
    q->tail = 0;
    q->mutex = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));

    ret_count = pthread_mutex_init(q->mutex, NULL);
    if (ret_count) {
        fprintf(stderr,
                "ERROR: Return code from pthread_mutex_init() is %d\n",
                ret_count);
        exit(EXIT_FAILURE);
    }

    q->notFull = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(q->notFull, NULL);
    q->notEmpty = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(q->notEmpty, NULL);

    return q;
}

void queueDelete(queue *q) {
    ret_count = pthread_mutex_destroy(q->mutex);
    if (ret_count) {
        fprintf(stderr,
                "ERROR: Return code from pthread_mutex_destroy() is %d\n",
                ret_count);
        exit(EXIT_FAILURE);
    }

    free(q->mutex);

    pthread_cond_destroy(q->notFull);
    free(q->notFull);
    pthread_cond_destroy(q->notEmpty);
    free(q->notEmpty);

    free(q);
}

void queueAdd(queue *q, int in) {
    q->buff[q->tail] = in;
    q->tail++;

    /* Make Circular Queue */
    if (q->tail == QUEUESIZE)
        q->tail = 0;
    /* Queue is Full */
    if (q->tail == q->head)
        q->full = 1;
    q->empty = 0;
}

void queueDel(queue *q, int *out) {
    *out = q->buff[q->head];
    q->head++;
    if (q->head == QUEUESIZE)
        q->head = 0;
    if (q->head == q->tail)
        q->empty = 1;
    q->full = 0;
}

void *producer(void *q) {
    queue *fifo;
    int i;

    fifo = (queue *) q;

    for (i = 0; i < LOOP; i++) {
        /* TODO First, we need lock */
        pthread_mutex_lock(fifo->mutex);
        /* TODO if fifo is not full... escape this while loop */
        while (fifo->full) {
            printf("producer: queue FULL. \n");
            pthread_cond_wait(fifo->notFull, fifo->mutex);
        }
        queueAdd(fifo, i);
        /* TODO Finally, we need unlock */
        pthread_mutex_unlock(fifo->mutex);
        pthread_cond_signal(fifo->notEmpty);
        printf("producer: add %d\n", i);
        sleep(3);
    }

    return NULL;
}

void *consumer(void *q) {
    queue *fifo;
    int i, d;

    fifo = (queue *) q;

    for (i = 0; i < LOOP; i++) {
        /* TODO First, we need lock */
        pthread_mutex_lock(fifo->mutex);
        /* TODO if fifo is not empty... escape this while loop */
        while (fifo->empty) {
            printf("producer: queue EMPTY. \n");
            pthread_cond_wait(fifo->notEmpty, fifo->mutex);
        }
        queueDel(fifo, &d);
        /* TODO Finally, we need unlock */
        pthread_mutex_unlock(fifo->mutex);
        pthread_cond_signal(fifo->notFull);
        printf("producer: delete %d\n", d);
        sleep(5);
    }

    return NULL;
}

int main() {
    queue *fifo;
    pthread_t prod, cons;

    // TODO Queue initialization
    fifo = queueInit();
    if (fifo == NULL) {
        fprintf(stderr, "main: Queue Initialization Failed.\n");
        exit(EXIT_FAILURE);
    }

    // TODO Thread Creation: prod & cons
    ret_count = pthread_create(&prod, NULL, producer, fifo);
    if (ret_count) {
        fprintf(stderr,
                "ERROR: Return code from pthread_create(&prod) is %d\n",
                ret_count);
        exit(EXIT_FAILURE);
    }

    ret_count = pthread_create(&cons, NULL, consumer, fifo);
    if (ret_count) {
        fprintf(stderr,
                "ERROR: Return code from pthread_create(&cons) is %d\n",
                ret_count);
        exit(EXIT_FAILURE);
    }

    ret_count = pthread_join(prod, NULL);
    if (ret_count) {
        fprintf(stderr,
                "ERROR: Return code from pthread_join(prod) is %d\n",
                ret_count);
        exit(EXIT_FAILURE);
    }

    ret_count = pthread_join(cons, NULL);
    if (ret_count) {
        fprintf(stderr,
                "ERROR: Return code from pthread_join(cons) is %d\n",
                ret_count);
        exit(EXIT_FAILURE);
    }

    queueDelete(fifo);

    return 0;
}