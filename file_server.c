// file_server.c : TCP Server to receive file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUF_SIZE];
    FILE *fp;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Server address setup
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen
    listen(server_fd, 1);
    printf("Server running... Waiting for connection...\n");

    // Accept connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
        perror("Accept error");
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    // Open file to write received data
    fp = fopen("received_file.txt", "wb");
    if (!fp) {
        perror("File error");
        exit(EXIT_FAILURE);
    }

    // Receive file data
    ssize_t bytes;
    while ((bytes = recv(new_socket, buffer, BUF_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes, fp);
    }

    printf("File received successfully.\n");

    fclose(fp);
    close(new_socket);
    close(server_fd);

    return 0;
}
