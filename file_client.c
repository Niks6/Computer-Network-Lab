// file_client.c : TCP Client to send file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE];
    FILE *fp;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    // Server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IP to binary (server is localhost)
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("IP error");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connect failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    // Open file to send
    fp = fopen("send_file.txt", "rb");
    if (!fp) {
        perror("File not found");
        exit(EXIT_FAILURE);
    }

    // Read and send file data
    size_t bytes;
    while ((bytes = fread(buffer, 1, BUF_SIZE, fp)) > 0) {
        send(sock, buffer, bytes, 0);
    }

    printf("File sent successfully.\n");

    fclose(fp);
    close(sock);

    return 0;
}
