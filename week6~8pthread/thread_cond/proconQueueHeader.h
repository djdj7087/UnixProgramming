//
// Created by wooyoung on 4/26/20.
//

#ifndef PTHREADEXIT_PROCONQUEUEHEADER_H
#define PTHREADEXIT_PROCONQUEUEHEADER_H

// define Queue size
#define QUEUESIZE 10
// define # of iterations
#define LOOP 20

typedef struct {
    int buff[QUEUESIZE];
    int head, tail;
    int full, empty;
    pthread_mutex_t *mutex;
    pthread_cond_t *notFull;
    pthread_cond_t *notEmpty;
} queue;

// Queue operations
queue *queueInit(void);

void queueDelete(queue *q);         // Destroy queue

void queueDel(queue *q, int *out);  // Fetch data

void queueAdd(queue *q, int in); // Add data

// Threads
void *producer(void *arg);

void *consumer(void *arg);

#endif //PTHREADEXIT_PROCONQUEUEHEADER_H
