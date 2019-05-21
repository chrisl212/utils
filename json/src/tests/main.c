//
//  main.c
//  utils
//
//  Created by Christopher Loonam on 5/20/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#include <stdio.h>
#include "../json/json.h"

void format(const char *fpath, const char *opath) {
    json_t json = json_loadFromPath(fpath);
    FILE *fp = fopen(opath, "w");
    json_write(json, fp);
    fclose(fp);
    json_free(json);
}

int main(int argc, char **argv) {
    int i;
    char *fpath = NULL;
    char *output = NULL;
    char sw = ' ';

    if (argc < 3) {
        printf("json [-f|-h] file [-o output]\n");
        return -1;
    }

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && !fpath)
            sw = argv[i][1];
        else if (argv[i][0] != '-')
            fpath = output = argv[i];
        else if (argv[i][0] == '-' && fpath && argv[i][1] == 'o')
            output = argv[++i];
    }

    if (sw == 'f') {
        format(fpath, output);
    } else if (sw == 'h') {
        printf("json [-f|-h] file [-o output]\n");
    }

    return 0;
}
