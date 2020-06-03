//
// Created by djdj7 on 2020-06-03.
//

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main() {

    char *f_name = "test.txt";
    int fd1, fd2, count;
    char buf[30];

    fd1 = open(f_name, O_RDONLY);

    printf("fd1: %d\n", fd1);
    count = read(fd1, buf, 12);
    printf("fd1's first printf: %s\n", buf);

    close(fd1);

    return 0;
}