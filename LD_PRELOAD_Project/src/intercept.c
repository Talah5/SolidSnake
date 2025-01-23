#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <openssl/ssl.h>

// Déclarations des fonctions originales
int (*original_SSL_read)(SSL *ssl, void *buf, int num) = NULL;
int (*original_SSL_write)(SSL *ssl, const void *buf, int num) = NULL;

// Interception de SSL_read
int SSL_read(SSL *ssl, void *buf, int num) {
    if (!original_SSL_read) {
        original_SSL_read = dlsym(RTLD_NEXT, "SSL_read");
        if (!original_SSL_read) {
            fprintf(stderr, "Error: Could not load original SSL_read: %s\n", dlerror());
            return -1;
        }
    }

    int result = original_SSL_read(ssl, buf, num);

    if (result > 0) {
        printf("[Intercepted SSL_read] Data: %.*s\n", result, (char *)buf);
    }

    return result;
}

// Interception de SSL_write
int SSL_write(SSL *ssl, const void *buf, int num) {
    if (!original_SSL_write) {
        original_SSL_write = dlsym(RTLD_NEXT, "SSL_write");
        if (!original_SSL_write) {
            fprintf(stderr, "Error: Could not load original SSL_write: %s\n", dlerror());
            return -1;
        }
    }

    int result = original_SSL_write(ssl, buf, num);

    if (result > 0) {
        printf("[Intercepted SSL_write] Data: %.*s\n", result, (char *)buf);
    }

    return result;
}
