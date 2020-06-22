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
    fd2 = open(f_name, O_RDONLY);

    printf("fd1: %d\n", fd1);
    printf("fd2: %d\n", fd2);
    count = read(fd1, buf, 12);
    printf("fd1's first printf: %s\n", buf);
    lseek(fd1, 1, SEEK_CUR);
    count = read(fd1, buf, 12);
    printf("fd1's second printf: %s\n", buf);

    count = read(fd2, buf, 12);
    printf("fd2's first printf: %s\n", buf);
    lseek(fd2, 1, SEEK_CUR);
    count = read(fd2, buf, 12);
    printf("fd2's second printf: %s\n", buf);

    close(fd1);
    close(fd2);

    return 0;
}