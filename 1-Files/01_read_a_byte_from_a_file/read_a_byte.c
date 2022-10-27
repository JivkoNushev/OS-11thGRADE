#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
    int fd = open("./text.txt", O_RDONLY);

    if (-1 == fd)
    {
        printf("Can't open file");
        exit(1);
    }

    char read_buffer[1];
    int read_state = read(fd, (void*)read_buffer, 1);
    if (-1 == read_state)
    {
        printf("Can't read file");
        exit(2);
    }
    if (1 != read_state)
    {
        printf("Didn't read file correctly");
        exit(3);
    }


    int write_state = write(1, (void*)read_buffer, 1);
    if(-1 == write_state)
    {
        printf("Can't write to stdout");
        exit(4);
    }

    return 0;
}