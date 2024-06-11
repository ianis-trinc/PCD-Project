#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in servAddr;
    char buffer[1024] = {0};
    char input[1024];

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);

    // Convert server address
    if (inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr) <= 0) {
        perror("Invalid address");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    // Send initial identification message
    char initMessage[] = "Admin client";
    send(sock, initMessage, strlen(initMessage), 0);

    // Wait for user input
    while (1) {
        printf("Enter command [close, message]: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;  // Remove newline character

        if (strcmp(input, "close") == 0) {
            printf("CLOSE\n");
            send(sock, input, strlen(input), 0);
            break;
        }

        // Send message
        send(sock, input, strlen(input), 0);
        printf("Message sent: %s\n", input);

        // Wait for response
        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, sizeof(buffer) - 1);
        printf("Received response: %s\n", buffer);
    }

    // Close connection
    close(sock);
    return 0;
}