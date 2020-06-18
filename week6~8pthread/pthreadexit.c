#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void *thread_function(void *data) {
    /* Return Value (Must Static or Global Variable) */
    static int retval = 999;
    int i = 0;
    int mycount = *((int *)data);

    while (1) {
        if(i == mycount){
            /* Exit this Thread!! */
            pthread_exit((void*)&retval);
        }
        printf("Thread Running... %d: data = %d\n", i, mycount);
        i++;
        sleep(1);
    }
}

int main() {
    pthread_t p_thread;
    void *threadret = NULL;
    int ret;                    // return value
    int count = 3;

    ret = pthread_create(&p_thread, NULL, thread_function, (void *)&count);
    if (ret < 0) {
        perror("week6_thread create error: ");
    }

    pthread_join(p_thread, &threadret);

    printf("week6_thread exit code: %d\n", *((int *)threadret));
    printf("main week6_thread is finished...\n");
    return 0;
}
