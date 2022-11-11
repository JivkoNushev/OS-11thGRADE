#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <errno.h>
#include <err.h>

#include "../3-utils/utils.h"
#include "../3-utils/utils.c"

int main(int argc, char const *argv[])
{
    int fd = open(argv[1], O_RDONLY);
    if(-1 == fd)
    {
        err(EXIT_FAILURE, NULL);
    }

    uint32_t numbers[(size_t)1e8];
    uint32_t block[256];
    size_t index = 0;
    int read_status = 0;
    puts("a");
    while (0 < (read_status = read(fd, block, sizeof block)))
    {
        printf("%d\n", read_status / sizeof(uint32_t));
        for (size_t i = 0; i < read_status / sizeof(uint32_t); i++)
        {
            numbers[i + index] = block[i];
        }
        index += read_status / sizeof(uint32_t);
    }
    if(-1 == read_status)
    {
        close(fd);
        err(EXIT_FAILURE, NULL);
    }
    
    for (size_t i = 0; i < index; i++)
    {
        printf("%32u, ", numbers[i]);
    }
    

    return 0;
}
