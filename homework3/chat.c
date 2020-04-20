/*
 * chat.c -- CP33357 assignment 3, Spring 2020
 * Choi Woo Young, id #201524600, section 059, TAK. SUNG WOO
 */
#include "mychatheader.h"

int is_running;                     // Program Running Flag
CHAT_INFO *chatInfo = NULL;         // Shared Memory for Chat Info
char userID[20];                    // Global variable for user's ID

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "[Usage]: ./shmwrite UserID \n");
        exit(EXIT_STATUS);
    } else {
        strcpy(userID, argv[1]);

        initscr();                          // initializing main terminal screen

        getmaxyx(stdscr, row, col);         // get window's row_size and col_size

        chat();                             // call main chatting function

        endwin();
    }
    return 0;
}

void chat() {
    key_t shm_key = 201524600;          // Shared memory key
    int shm_id;                         // Shared memory id
    void *shmAddr;                      // Shared memory Address

    /*
     * Make New Windows
     * newwin(row_size, col_size, row_start, col_start)
     * row = 24 / col = 8 / INPUT_WINDOW_H = 4 / INPUT_WINDOW_W = 60
     */
    input_scr   = newwin(INPUT_WINDOW_H, INPUT_WINDOW_W, row - INPUT_WINDOW_H, 0);
    output_scr  = newwin(row - INPUT_WINDOW_H, INPUT_WINDOW_W, 0, 0);
    logging_scr = newwin(row - INPUT_WINDOW_H, col - INPUT_WINDOW_W, 0, INPUT_WINDOW_W);
    time_scr    = newwin(INPUT_WINDOW_H, col - INPUT_WINDOW_W, row - INPUT_WINDOW_H, INPUT_WINDOW_W);

    shm_id = shmget(shm_key, sizeof(CHAT_INFO),SHM_RW_FLAG | IPC_CREAT | IPC_EXCL);

    if (shm_id < 0) {
        shm_id = shmget(shm_key, sizeof(CHAT_INFO), SHM_RW_FLAG);
        shmAddr = shmat(shm_id, (void *) 0, SHM_RW_FLAG);

        if (shmAddr < (void *) 0) {
            perror("shm attach is failed: ");
            exit(EXIT_STATUS);
        }
    } else {
        shmAddr = shmat(shm_id, (void *) 0, SHM_RW_FLAG);
    }
    chatInfo = (CHAT_INFO *) shmAddr;

    mvwprintw(output_scr, 0, 1, "\n ***** Type /bye to quit!! ***** \n\n");
    print_chat();
    draw_box();
    scrollok(output_scr, TRUE);

    buff_in.id = 0;
    buff_out.id = 0;
    is_running = 1;

    /* declare for thread */
    pthread_t thread[4];

    pthread_create(&thread[0], NULL, get_input, NULL);
    pthread_create(&thread[1], NULL, recv_send, NULL);
    pthread_create(&thread[2], NULL, print_time, NULL);
    pthread_create(&thread[3], NULL, print_logging, NULL);

    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    pthread_join(thread[2], NULL);
    pthread_join(thread[3], NULL);

    delwin(input_scr);
    delwin(output_scr);
    delwin(logging_scr);
    delwin(time_scr);
}

void *get_input() {
    char tmp[BUFFSIZE];

    while (is_running) {
        mvwgetstr(input_scr, 1, 1, tmp);

        /* If user typed "/bye", exit the chat program... */
        if (!strcmp(tmp, "/bye")) {
            chatInfo->totalMessageCount++;
            strcpy(buff_in.userID, userID);
            strcpy(buff_in.message, "quit");
            buff_in.id = chatInfo->totalMessageCount;
            chatInfo->messageContainer[chatInfo->totalMessageCount] = buff_in;
            die("bye");
        } else { /* else if user typed another words */
            chatInfo->totalMessageCount++;
            strcpy(buff_in.userID, userID);
            strcpy(buff_in.message, tmp);
            buff_in.id = chatInfo->totalMessageCount;
            chatInfo->messageContainer[chatInfo->totalMessageCount] = buff_in;
        }

        /* Erase Typed Message!! */
        werase(input_scr);
        draw_box();

        usleep(100);
    }
    return NULL;
}

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
    if(chatInfo->totalPersonCount == 0) {
        strcpy(chatInfo->personContainer[chatInfo->totalPersonCount].userID, userID);
        chatInfo->totalPersonCount += 1;
    } else {
        for(int i = 0 ; i < chatInfo->totalPersonCount ; i++) {
            if(!strcmp(chatInfo->personContainer[i].userID, userID)) {
                isLoggedIn = true;
            }
        }
        /* if user is not logged in, user can log in */
        if(!isLoggedIn) {
            strcpy(chatInfo->personContainer[chatInfo->totalPersonCount].userID, userID);
            chatInfo->totalPersonCount += 1;
        }
    }

    while (is_running) {
        /*
         * if total person count and shared memory's total person count is different,
         * we can notice new person is logged in...
         */
        if(totalPersonCount != chatInfo->totalPersonCount) {
            werase(logging_scr);
            totalPersonCount = chatInfo->totalPersonCount;
            for(int i = 0 ; i < totalPersonCount ; i++) {
                wprintw(logging_scr, "\n  **[%s]**  \n",
                        chatInfo->personContainer[i].userID);
                draw_box();
            }
        }
    }
    return NULL;
}

void print_chat() {
    /* If person quit and reenter chat room... previous chatting is showed */
    for(int i = 1 ; i < chatInfo->totalMessageCount ; i++) {
        if(!strcmp(chatInfo->messageContainer[i].message, "quit")) {
            wprintw(output_scr, " %s is out ...\n", chatInfo->messageContainer[i].userID);
            continue;
        }
        wprintw(output_scr, " [%s: %d] > %s\n",
                chatInfo->messageContainer[i].userID,
                chatInfo->messageContainer[i].id,
                chatInfo->messageContainer[i].message);
    }
}

void *recv_send() {
    struct message oldmsg;

    while (is_running) {
        if(chatInfo->totalMessageCount == 0) { continue; }
        else {
            buff_out = chatInfo->messageContainer[chatInfo->totalMessageCount];
            /* If we got new messages */
            if (oldmsg.id != buff_out.id) {
                if(!strcmp(buff_out.message, "quit")) {
                    wprintw(output_scr, " %s is out ...\n", buff_out.userID);
                    oldmsg.id = buff_out.id;
                    continue;
                }
                wprintw(output_scr, " [%s: %d] > %s\n", buff_out.userID, buff_out.id, buff_out.message);
                oldmsg.id = buff_out.id;
                draw_box();
            }
        }
    }
    return NULL;
}

void die(char *s) {
    int personIndex = 0;
    /* If total person count is one... when user quit, we need to remove shared memory */
    if(chatInfo->totalPersonCount == 1) {
        chatremove();
    } else if(chatInfo->totalPersonCount > 1){
        for(int i = 0 ; i < chatInfo->totalPersonCount ; i++) {
            if(!strcmp(chatInfo->personContainer[i].userID, userID)) {
                personIndex = i;
            } else { continue; }
        }
        /* Erase Person's Information when user quit... */
        for(int i = personIndex ; i < chatInfo->totalPersonCount-1 ; i++) {
            chatInfo->personContainer[i] = chatInfo->personContainer[i+1];
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