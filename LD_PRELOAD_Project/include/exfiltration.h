#ifndef EXFILTRATION_H
#define EXFILTRATION_H

#include <stddef.h>

void exfiltrate_to_server(const char *data, size_t size);

#endif // EXFILTRATION_H
