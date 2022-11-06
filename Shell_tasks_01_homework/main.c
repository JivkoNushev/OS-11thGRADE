#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "../3-utils/utils.h"
#include "../3-utils/utils.c"

#include "wc.h"
#include "head.h"
#include "tail.h"

int main(int argc, const char *argv[])
{
    int fd = open(argv[1], O_RDONLY);
    if(0 == fd)
    {
        puts("Couldn't open file");
        exit(1);
    }
    //wc(fd, argc, argv);
    // head(fd, argc, argv);
    tail(fd, argc, argv);

    return 0;
}
