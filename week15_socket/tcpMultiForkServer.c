//
// Created by wooyoung on 6/22/20.
//

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    // Define socket address internet structure.
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    int server_socket;

    int client_socket;
    int client_address_size;

    /* argc means number of arguments. */

    if (argc != 2) {
        printf("usage: %s (Port)\n", argv[0]);
        exit(0);
    }

    if ((server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("socket error: ");
        exit(0);
    }

    printf("[Debug] server_socket: %d\n", server_socket);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[1]));

    if (bind(server_socket, (struct sockaddr *)&server_address,
             sizeof(server_address)) < 0) {
        perror("bind error: ");
        exit(0);
    }

    if (listen(server_socket, 5) < 0) {
        perror("bind error: ");
        exit(0);
    }

    client_address_size = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address,
            &client_address_size);

    printf("[Debug] There is a connection request from a client\n");

    close(server_socket);

    return 0;
}