#include <stdio.h>
#include "../json/json.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("json [file]\n");
        return -1;
    }
    json_t json = json_loadFromPath(argv[1]);
    json_write(json, stdout);
    json_free(json);
    return 0;
}
