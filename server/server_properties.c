#include "skyelib.h"

void parse_server_properties(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {

        char fullpath[256];
        snprintf(fullpath, sizeof(fullpath), "bin/%s", filename);
        fp = fopen(fullpath, "r");

        if (!fp){
            perror("Failed to open server.properties");
            return;
        }
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), fp)) {
        // Skip comments and blank lines
        if (line[0] == '#' || line[0] == '\n') continue;

        // Find '=' delimiter
        char *equals = strchr(line, '=');
        if (!equals) continue;

        *equals = '\0';
        char *key = trim(line);
        char *value = trim(equals + 1);

        if (strcmp(key, "server-ip") == 0 && strlen(value) > 0)
            strncpy(SERVER_IP, value, sizeof(SERVER_IP) - 1);

        else if (strcmp(key, "server-port") == 0)
            SERVER_PORT = atoi(value);

        else if (strcmp(key, "nogui") == 0)
            SERVER_HEADLESS = atoi(value);
    }

    fclose(fp);
}