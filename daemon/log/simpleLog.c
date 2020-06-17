//
// Created by djdj7 on 2020-06-17.
//

#include <syslog.h>

int main() {

    /* Log Text Area */
    openlog("Logs", LOG_PID, LOG_USER);

    syslog(LOG_INFO, "Start Logging");

    closelog();

    return 0;
}