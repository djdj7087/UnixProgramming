//
// Created by djdj7 on 2020-06-05.
//

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main() {

    char *f_name = "test.txt";
    int fd1, fd2, count;
    char buf[30] = {'\0'};

    fd1 = open(f_name, O_RDONLY);
    fd2 = dup(fd1);

    printf("fd1: %d\n", fd1);

    count = read(fd1, buf, 12);
    printf("fd1's first printf: %s\n", buf);
    lseek(fd1, 1, SEEK_CUR);

    count = read(fd2, buf, 12);
    printf("fd2's first printf: %s\n", buf);

    close(fd1);
    close(fd2);

    return 0;
}