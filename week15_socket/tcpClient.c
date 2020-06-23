//
// Created by wooyoung on 6/22/20.
//

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char **argv) {
    int client_socket;                  // Define Client Socket.
    struct sockaddr_in client_address;  // Define socket address internet structure

    if (argc != 3) {
        printf("Usage: %s (IP_Address) (Port)\n", argv[0]);
        exit(0);
    }

    if ((client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("socket error: ");
        exit(0);
    }

    printf("[Debug] client socket: %d\n", client_socket);

    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(argv[1]);
    client_address.sin_port = htons(atoi(argv[2]));

    if (connect(client_socket, (struct sockaddr *)&client_address,
            sizeof(client_address)) < 0) {
        perror("connect error: ");
        exit(0);
    }

    close(client_socket);

    return 0;
}