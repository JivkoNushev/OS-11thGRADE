#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int fd = open("../04_write_numbers_into_file/numbers", O_RDONLY);
    if (-1 == fd)
    {
        printf("Can't open file");
        close(fd);
        exit(1);
    }

    int read_status;
    int buffer;
    for(int counter = 0; sizeof(int) == (read_status = read(fd, &buffer, sizeof(int))); counter++)
        printf("number %d is %d\n", counter, buffer);
    if(-1 == read_status)
    {
        printf("Can't read file");
        close(fd);
        exit(2);
    }

    return 0;
}
