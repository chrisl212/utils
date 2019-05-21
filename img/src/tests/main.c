//
//  main.c
//  utils
//
//  Created by Christopher Loonam on 5/20/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#include <stdio.h>
#include "../img/img.h"

int main(int argc, char **argv) {
    img_t img;
    uint32_t width, height;
    
    if (argc != 2) {
        printf("img [FILE]\n");
        return -1;
    }

    img = img_loadFromPath(argv[1]);
    img_getDimensions(img, &width, &height);
    printf("%dx%d\n", width, height);
    
    img_free(img);
    return 0;
}
