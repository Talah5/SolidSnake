#ifndef INTERCEPT_H
#define INTERCEPT_H

#include <stddef.h>

ssize_t read(int fd, void *buf, size_t count);

#endif // INTERCEPT_H
