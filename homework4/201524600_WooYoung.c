/*
 * 201524600_WooYoung.c -- CP33357 assignment 4, Spring 2020
 * Choi Woo Young, id #201524600, section 059, TAK. SUNG WOO
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>

#define INPUT_WINDOW_H      4       // Input window height
#define INPUT_WINDOW_W      60      // Input window width
#define SHM_RW_FLAG         0666    // Shared memory Read Write Flag

#define BUFFSIZE 1024               // Message Buffer Size
#define QUEUESIZE 10                // Message Container Queue Size

typedef struct message {
    long id;                        // message id
    char userID[20];                // message owner's ID
    char message[100];              // message strings
} MESSAGE_INFO;

typedef struct person {
    char userID[20];                // user's ID
} PERSON_INFO;

typedef struct {
    MESSAGE_INFO messageContainer[BUFFSIZE];    // Shared memory for messages
    PERSON_INFO personContainer[BUFFSIZE];      // Shared memory for persons
    int totalMessageCount;                      // Number of Messages
    int totalPersonCount;                       // Number of Persons
} CHAT_INFO;

typedef struct {
    MESSAGE_INFO fetchedMessage[QUEUESIZE];     // Fetched from shared memory
    int head, tail;
    int full, empty;
    pthread_mutex_t *mutex;                     // mutex for thread
    pthread_cond_t *notFull;                    // thread condition for not Full
    pthread_cond_t *notEmpty;                   // thread condition for not Empty
} FETCHED_MESSAGE_QUEUE;

// Function Prototype for fetch message from shared memory
void *FetchMessageFromShmThread(void *q);

// Function Prototype for display message
void *DisplayMessageThread(void *q);

// Function Prototype for print time
void *print_time();

// Function Prototype for print logging in users
void *print_logging();

// Function Prototype for get input
void *get_input();

// Function Prototype for auto input
void *auto_input();

// Function Prototype for draw and refresh boxes
void *draw_box();

// Function Prototype for erase shared memory
void chat_remove();

// Function Prototype for exit program
void die(char *s);

// Function Prototype for chatting main
void chat();

// Function Prototype for init queue
FETCHED_MESSAGE_QUEUE *queueInit();

// Function Prototype for delete queue
void queueDelete(FETCHED_MESSAGE_QUEUE *q);

// Function Prototype for add message into queue
void queueAdd(FETCHED_MESSAGE_QUEUE *q, MESSAGE_INFO m);

// Function Prototype for pop message from queue
MESSAGE_INFO queuePop(FETCHED_MESSAGE_QUEUE *q);

int is_running;                     // Program Running Flag
CHAT_INFO *chatInfo = NULL;         // Shared Memory for Chat Info
char userID[20];                    // Global variable for user's ID

int row;                            // terminal max row
int col;                            // terminal max col

struct message buff_in;             // message buffer for input
struct message buff_out;            // message buffer for output

WINDOW *input_scr;                  // Window for input
WINDOW *output_scr;                 // Window for output
WINDOW *logging_scr;                // Window for Logging in Accounts
WINDOW *time_scr;                   // Window for elapsed time

key_t shm_key;                      // Shared memory key
int shm_id;                         // Shared memory id
void *shmAddr;                      // Shared memory Address

sem_t *sem_chat;                    // Semaphore for Shared memory

int ret_count;                      // return count for queue

/* Function for init queue */
FETCHED_MESSAGE_QUEUE *queueInit(void) {
    FETCHED_MESSAGE_QUEUE *q;

    q = (FETCHED_MESSAGE_QUEUE *) malloc(sizeof(FETCHED_MESSAGE_QUEUE));
    if (q == NULL)
        return NULL;

    q->empty = 1;       // 1 means Yes or True
    q->full = 0;        // 0 means No or Flase
    q->head = 0;
    q->tail = 0;
    q->mutex = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));

    ret_count = pthread_mutex_init(q->mutex, NULL);
    if (ret_count) {
        fprintf(stderr, "ERROR: Return code from pthread_mutex_init() is %d\n", ret_count);
        exit(EXIT_FAILURE);
    }

    q->notFull = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(q->notFull, NULL);
    q->notEmpty = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(q->notEmpty, NULL);

    return q;
}

/* Function for push data into queue */
void queueAdd(FETCHED_MESSAGE_QUEUE *q, MESSAGE_INFO m) {
    q->fetchedMessage[q->tail] = m;
    q->tail++;

    /* Make Circular Queue */
    if (q->tail == QUEUESIZE) {
        q->tail = 0;
    }
    /* If Queue is Full */
    if (q->tail == q->head) {
        q->full = 1;
    }
    q->empty = 0;
}

/* Function for pop data from queue */
MESSAGE_INFO queuePop(FETCHED_MESSAGE_QUEUE *q) {
    MESSAGE_INFO messageInfo;
    messageInfo = q->fetchedMessage[q->head];
    q->head++;

    if (q->head == QUEUESIZE) {
        q->head = 0;
    }
    if (q->head == q->tail) {
        q->empty = 1;
    }
    q->full = 0;

    return messageInfo;
}

/* Function for Deallocate(free) queue */
void queueDelete(FETCHED_MESSAGE_QUEUE *q) {
    ret_count = pthread_mutex_destroy(q->mutex);
    if (ret_count) {
        fprintf(stderr, "ERROR: Return code from pthread_mutex_destroy() is %d\n", ret_count);
        exit(EXIT_FAILURE);
    }

    free(q->mutex);

    pthread_cond_destroy(q->notFull);
    free(q->notFull);
    pthread_cond_destroy(q->notEmpty);
    free(q->notEmpty);

    free(q);
}

/* Function for print time and elapsed time */
void *print_time() {
    /* For hh-mm-ss */
    int hour = 0;
    int minute = 0;
    int second = 0;
    int count = 0;

    while (is_running) {
        /* Variables for Current Time */
        time_t current_time = time(NULL);
        struct tm *time_info = localtime(&current_time);
        int current_second = time_info->tm_sec;
        int current_minute = time_info->tm_min;
        int current_hour = time_info->tm_hour;

        mvwprintw(time_scr, 1, 1, "current:%2d-%2d-%2d",
                  current_hour,
                  current_minute,
                  current_second);
        mvwprintw(time_scr, 2, 1, "elapsed:%2d-%2d-%2d",
                  hour,
                  minute,
                  second);

        draw_box();

        if (count % 2 == 0) {
            second++;
        }
        if (second == 60) {
            second = 0;
            minute++;
        }
        if (minute == 60) {
            minute = 0;
            hour++;
        }
        count++;
        usleep(500000);
    }
    return NULL;
}

/* Function for print logging in persons */
void *print_logging() {
    bool isLoggedIn = false;            // Logged In Flag for not allow duplication
    int totalPersonCount = 0;           // Variable for count total person number

    /* If person count is zero -> must initialize user */
    if (chatInfo->totalPersonCount == 0) {
        sem_wait(sem_chat);
        strcpy(chatInfo->personContainer[chatInfo->totalPersonCount].userID, userID);
        chatInfo->totalPersonCount += 1;
        sem_post(sem_chat);
    } else {
        sem_wait(sem_chat);
        for (int i = 0; i < chatInfo->totalPersonCount; i++) {
            if (!strcmp(chatInfo->personContainer[i].userID, userID)) {
                isLoggedIn = true;
            }
        }
        /* if user is not logged in, user can log in */
        if (!isLoggedIn) {
            strcpy(chatInfo->personContainer[chatInfo->totalPersonCount].userID, userID);
            chatInfo->totalPersonCount += 1;
        }
        sem_post(sem_chat);
    }

    while (is_running) {
        /*
         * if total person count and shared memory's total person count is different,
         * we can notice new person is logged in...
         */
        sem_wait(sem_chat);
        if (totalPersonCount != chatInfo->totalPersonCount) {
            werase(logging_scr);
            totalPersonCount = chatInfo->totalPersonCount;
            for (int i = 0; i < totalPersonCount; i++) {
                wprintw(logging_scr, "\n  **[%s]**  \n",
                        chatInfo->personContainer[i].userID);
                draw_box();
            }
        }
        sem_post(sem_chat);
    }
    return NULL;
}

/* Function define for get_input() */
void *get_input() {
    char tmp[BUFFSIZE];

    while (is_running) {
        mvwgetstr(input_scr, 1, 1, tmp);

        sem_wait(sem_chat);
        /* If user typed "/bye", exit the chat program... */
        if (!strcmp(tmp, "/bye")) {
            chatInfo->totalMessageCount++;
            strcpy(buff_in.userID, userID);
            strcpy(buff_in.message, "quit");
            buff_in.id = chatInfo->totalMessageCount;
            chatInfo->messageContainer[chatInfo->totalMessageCount] = buff_in;
            die("bye");
        } else { /* else if user typed other words */
            chatInfo->totalMessageCount++;
            strcpy(buff_in.userID, userID);
            strcpy(buff_in.message, tmp);
            buff_in.id = chatInfo->totalMessageCount;
            chatInfo->messageContainer[chatInfo->totalMessageCount] = buff_in;
        }
        sem_post(sem_chat);

        /* Erase Typed Message!! */
        werase(input_scr);
        draw_box();
    }
    return NULL;
}

/* Function define for auto_input() */
void *auto_input() {
    int auto_msg_count = 0;
    char tmp[BUFFSIZE];
    char num[BUFFSIZE];

    while (is_running) {
        srand((unsigned int) time(NULL) + (unsigned int)getpid());
        int random_sec = (rand() % 1001) + 1000;

        sem_wait(sem_chat);
        if (!strcmp("Jico", userID)) {
            chatInfo->totalMessageCount++;
            auto_msg_count++;
            strcpy(buff_in.userID, userID);
            strcpy(tmp, "Hello!! My name is Jico\n I love to sing any song. - ");
            sprintf(num, "%d\n", auto_msg_count);
            strcat(tmp, num);
            strcpy(buff_in.message, tmp);
            buff_in.id = chatInfo->totalMessageCount;
            chatInfo->messageContainer[chatInfo->totalMessageCount] = buff_in;
        } else if (!strcmp("Izzy", userID)) {
            chatInfo->totalMessageCount++;
            auto_msg_count++;
            strcpy(buff_in.userID, userID);
            strcpy(tmp, "Hi!! I am Izzy\n I like to play on the stage. Ho. - ");
            sprintf(num, "%d\n", auto_msg_count);
            strcat(tmp, num);
            strcpy(buff_in.message, tmp);
            buff_in.id = chatInfo->totalMessageCount;
            chatInfo->messageContainer[chatInfo->totalMessageCount] = buff_in;
        }
        sem_post(sem_chat);
        usleep(random_sec * 1000);
    }
    return NULL;
}

/* Function for fetch messages from shared memory */
void *FetchMessageFromShmThread(void *q) {
    FETCHED_MESSAGE_QUEUE *message_q = (FETCHED_MESSAGE_QUEUE *) q;
    struct message oldmsg;

    while (is_running) {
        pthread_mutex_lock(message_q->mutex);
        sem_wait(sem_chat);
        while (message_q->full) {
            pthread_cond_wait(message_q->notFull, message_q->mutex);
        }
        if (chatInfo->totalMessageCount == 0) {
            pthread_mutex_unlock(message_q->mutex);
        } else {
            buff_out = chatInfo->messageContainer[chatInfo->totalMessageCount];
            /* If we got new messages, push into queue */
            if(oldmsg.id != buff_out.id) {
                queueAdd(message_q, buff_out);
                oldmsg.id = buff_out.id;
            }
        }
        pthread_mutex_unlock(message_q->mutex);
        pthread_cond_signal(message_q->notEmpty);
        sem_post(sem_chat);
    }
    return NULL;
}

/* Function for display messages fetched from queue */
void *DisplayMessageThread(void *q) {
    struct message oldmsg;
    FETCHED_MESSAGE_QUEUE *message_q = (FETCHED_MESSAGE_QUEUE *) q;

    while (is_running) {
        pthread_mutex_lock(message_q->mutex);
        while (message_q->empty) {
            pthread_cond_wait(message_q->notEmpty, message_q->mutex);
        }
        buff_out = queuePop(message_q);
        if(oldmsg.id != buff_out.id) {
            if(!strcmp(buff_out.message, "quit")) {
                wprintw(output_scr, " %s is out ...\n", buff_out.userID);
                oldmsg.id = buff_out.id;
            } else {
                wprintw(output_scr, " [%s: %d] > %s\n", buff_out.userID, buff_out.id, buff_out.message);
                oldmsg.id = buff_out.id;
            }
            draw_box();
        }
        pthread_mutex_unlock(message_q->mutex);
        pthread_cond_signal(message_q->notFull);
    }

    return NULL;
}

/* Function for draw boxes, and redraw and refresh boxes */
void *draw_box() {
    /* Draw all boxes */
    box(input_scr, ACS_VLINE, ACS_HLINE);
    box(output_scr, ACS_VLINE, ACS_HLINE);
    box(logging_scr, ACS_VLINE, ACS_HLINE);
    box(time_scr, ACS_VLINE, ACS_HLINE);

    /* Refresh all boxes */
    wrefresh(input_scr);
    wrefresh(output_scr);
    wrefresh(logging_scr);
    wrefresh(time_scr);

    return NULL;
}

/* Function for Erase Shared memory... */
void chat_remove() {
    int shm_temp_id = shmget(shm_key, sizeof(CHAT_INFO), SHM_RW_FLAG);

    if (shm_temp_id < 0) {
        printf("Cannot found Shared Memory : id [%d]\n", shm_temp_id);
        perror("shmget failed: ");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shm_temp_id, IPC_RMID, 0) < 0) {
        printf("Failed to delete shared memory\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Successfully delete shared memory\n");
    }
}

void die(char *s) {
    int personIndex = 0;
    /* If total person count is one... when user quit, we need to remove shared memory */
    if (chatInfo->totalPersonCount == 1) {
        chat_remove();
    } else if (chatInfo->totalPersonCount > 1) {
        for (int i = 0; i < chatInfo->totalPersonCount; i++) {
            if (!strcmp(chatInfo->personContainer[i].userID, userID)) {
                personIndex = i;
            } else { continue; }
        }
        /* Erase Person's Information when user quit... */
        for (int i = personIndex; i < chatInfo->totalPersonCount - 1; i++) {
            chatInfo->personContainer[i] = chatInfo->personContainer[i + 1];
        }
        chatInfo->totalPersonCount -= 1;
    }
    delwin(input_scr);
    delwin(output_scr);
    endwin();

//    chatremove();
    perror(s);
    exit(1);
}

void chat() {
    /*---- queue initialize ----*/
    FETCHED_MESSAGE_QUEUE *message_queue;

    message_queue = queueInit();
    if (message_queue == NULL) {
        fprintf(stderr, "chat: Queue Initialize Failed.\n");
        exit(EXIT_FAILURE);
    }

    /*---- semaphore ----*/
    sem_chat = (sem_t *) malloc(sizeof(sem_t));

    /*
     * Make New Windows
     * newwin(row_size, col_size, row_start, col_start)
     * row = 24 / col = 8 / INPUT_WINDOW_H = 4 / INPUT_WINDOW_W = 60
     */
    input_scr = newwin(INPUT_WINDOW_H, INPUT_WINDOW_W, row - INPUT_WINDOW_H, 0);
    output_scr = newwin(row - INPUT_WINDOW_H, INPUT_WINDOW_W, 0, 0);
    logging_scr = newwin(row - INPUT_WINDOW_H, col - INPUT_WINDOW_W, 0, INPUT_WINDOW_W);
    time_scr = newwin(INPUT_WINDOW_H, col - INPUT_WINDOW_W, row - INPUT_WINDOW_H, INPUT_WINDOW_W);

    shm_key = ftok("chat.c", 'R');
    shm_id = shmget(shm_key, sizeof(CHAT_INFO), SHM_RW_FLAG | IPC_CREAT | IPC_EXCL);

    /*---- If shm have already existed..... just get shm ----*/
    if (shm_id < 0) {
        shm_id = shmget(shm_key, sizeof(CHAT_INFO), SHM_RW_FLAG);
        shmAddr = shmat(shm_id, (void *) 0, SHM_RW_FLAG);

        if (shmAddr < (void *) 0) {
            perror("shm attach is failed: ");
            exit(EXIT_FAILURE);
        }
    } else {
        shmAddr = shmat(shm_id, (void *) 0, SHM_RW_FLAG);
    }
    chatInfo = (CHAT_INFO *) shmAddr;

    /*---- unlink semaphore before open semaphore ----*/
    sem_unlink("chatSem");

    /*---- named Semaphore ----*/
    sem_chat = sem_open("chatSem", O_CREAT | O_EXCL, 0644, 1);
    if (sem_chat == SEM_FAILED) {
        chat_remove();
        perror("sem_open [chatSem] : ");
        exit(EXIT_FAILURE);
    }

    mvwprintw(output_scr, 0, 1, "\n ***** Type /bye to quit!! ***** \n\n");
    draw_box();
    scrollok(output_scr, TRUE);

    buff_in.id = 0;
    buff_out.id = 0;
    is_running = 1;

    /* declare for threads */
    pthread_t getInput, autoInput;
    pthread_t printTime, printLogging;
    pthread_t producer, consumer;

    /* Create and Check threads */
    ret_count = pthread_create(&getInput, NULL, get_input, NULL);
    if (ret_count) {
        fprintf(stderr, "ERROR: Return code from pthread_create(&getInput) is %d\n", ret_count);
        exit(EXIT_FAILURE);
    }
    ret_count = pthread_create(&autoInput, NULL, auto_input, NULL);
    if (ret_count) {
        fprintf(stderr, "ERROR: Return code from pthread_create(&autoInput) is %d\n", ret_count);
        exit(EXIT_FAILURE);
    }
    ret_count = pthread_create(&printTime, NULL, print_time, NULL);
    if (ret_count) {
        fprintf(stderr, "ERROR: Return code from pthread_create(&printTime) is %d\n", ret_count);
        exit(EXIT_FAILURE);
    }
    ret_count = pthread_create(&printLogging, NULL, print_logging, NULL);
    if (ret_count) {
        fprintf(stderr, "ERROR: Return code from pthread_create(&printLogging) is %d\n", ret_count);
        exit(EXIT_FAILURE);
    }
    ret_count = pthread_create(&producer, NULL, FetchMessageFromShmThread, message_queue);
    if (ret_count) {
        fprintf(stderr, "ERROR: Return code from pthread_create(&producer) is %d\n", ret_count);
        exit(EXIT_FAILURE);
    }
    ret_count = pthread_create(&consumer, NULL, DisplayMessageThread, message_queue);
    if (ret_count) {
        fprintf(stderr, "ERROR: Return code from pthread_create(&consumer) is %d\n", ret_count);
        exit(EXIT_FAILURE);
    }

    /* Join and check Threads */
    ret_count = pthread_join(getInput, NULL);
    if (ret_count) {
        fprintf(stderr, "ERROR: Return code from pthread_join(&getInput) is %d\n", ret_count);
        exit(EXIT_FAILURE);
    }
    ret_count = pthread_join(autoInput, NULL);
    if (ret_count) {
        fprintf(stderr, "ERROR: Return code from pthread_join(&autoInput) is %d\n", ret_count);
        exit(EXIT_FAILURE);
    }
    ret_count = pthread_join(printTime, NULL);
    if (ret_count) {
        fprintf(stderr, "ERROR: Return code from pthread_join(&printTime) is %d\n", ret_count);
        exit(EXIT_FAILURE);
    }
    ret_count = pthread_join(printLogging, NULL);
    if (ret_count) {
        fprintf(stderr, "ERROR: Return code from pthread_join(&printLogging) is %d\n", ret_count);
        exit(EXIT_FAILURE);
    }
    ret_count = pthread_join(producer, NULL);
    if (ret_count) {
        fprintf(stderr, "ERROR: Return code from pthread_join(&producer) is %d\n", ret_count);
        exit(EXIT_FAILURE);
    }
    ret_count = pthread_join(consumer, NULL);
    if (ret_count) {
        fprintf(stderr, "ERROR: Return code from pthread_join(&consumer) is %d\n", ret_count);
        exit(EXIT_FAILURE);
    }

    delwin(input_scr);
    delwin(output_scr);
    delwin(logging_scr);
    delwin(time_scr);

    queueDelete(message_queue);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "[Usage]: ./shmwrite UserID \n");
        exit(EXIT_FAILURE);
    } else {
        strcpy(userID, argv[1]);

        initscr();                          // initializing main terminal screen

        getmaxyx(stdscr, row, col);         // get window's row_size and col_size

        chat();                             // call main chatting function

        endwin();
    }
    return 0;
}