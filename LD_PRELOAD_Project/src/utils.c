#include <stdio.h>
#include <time.h>

void log_data(const char *tag, const char *data, size_t size) {
    FILE *log_file = fopen("/tmp/intercept.log", "a");
    if (log_file) {
        time_t now = time(NULL);
        fprintf(log_file, "[%s] %s: %.*s\n", ctime(&now), tag, (int)size, data);
        fclose(log_file);
    }
}
