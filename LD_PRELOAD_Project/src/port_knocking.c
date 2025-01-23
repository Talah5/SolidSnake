#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// Ports de la séquence à écouter
const int knock_sequence[] = {1234, 2345, 3456};
#define SEQUENCE_LENGTH 3

int main() {
    int knock_index = 0;
    int server_sockets[SEQUENCE_LENGTH];
    struct sockaddr_in server_addr;

    printf("Initialisation du système de port knocking...\n");

    // Crée un socket pour chaque port de la séquence
    for (int i = 0; i < SEQUENCE_LENGTH; i++) {
        server_sockets[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (server_sockets[i] < 0) {
            perror("Erreur lors de la création du socket");
            exit(EXIT_FAILURE);
        }

        // Configuration de l'adresse du socket
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY; // Écoute sur toutes les interfaces
        server_addr.sin_port = htons(knock_sequence[i]);

        // Liaison du socket au port
        if (bind(server_sockets[i], (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Erreur lors du bind");
            exit(EXIT_FAILURE);
        }

        // Mise en écoute
        if (listen(server_sockets[i], 1) < 0) {
            perror("Erreur lors de l'écoute");
            exit(EXIT_FAILURE);
        }

        printf("En écoute sur le port %d...\n", knock_sequence[i]);
    }

    printf("Système de port knocking prêt. Attente de la séquence...\n");

    // Boucle pour détecter la séquence
    while (1) {
        fd_set readfds;
        FD_ZERO(&readfds);

        // Ajoute les sockets à surveiller
        for (int i = knock_index; i < SEQUENCE_LENGTH; i++) {
            FD_SET(server_sockets[i], &readfds);
        }

        // Attend une connexion sur les ports
        int max_fd = server_sockets[SEQUENCE_LENGTH - 1];
        if (select(max_fd + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror("Erreur lors du select");
            exit(EXIT_FAILURE);
        }

        // Vérifie quel port a reçu une connexion
        for (int i = knock_index; i < SEQUENCE_LENGTH; i++) {
            if (FD_ISSET(server_sockets[i], &readfds)) {
                printf("Connexion détectée sur le port %d\n", knock_sequence[i]);
                knock_index++;

                // Vérifie si la séquence est complète
                if (knock_index == SEQUENCE_LENGTH) {
                    printf("Séquence correcte détectée ! Activation du malware.\n");
                    system("touch /tmp/activation_success"); // Exemple d'action
                    knock_index = 0; // Réinitialise la séquence
                }
                break;
            }
        }
    }

    // Fermeture des sockets
    for (int i = 0; i < SEQUENCE_LENGTH; i++) {
        close(server_sockets[i]);
    }

    return 0;
}
