#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "../3-utils/utils.h"
#include "../3-utils/utils.c"

#include "wc.h"

int main(int argc, const char *argv[])
{
    int fd = open("file", O_RDONLY);
    if(-1 == fd)
    {
        puts("Couldn't open file\n");
        exit(1);
    }

    wc(fd, argc, argv);

    close(fd);
    return 0;
}
