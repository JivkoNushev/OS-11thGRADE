#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int fd = open("data", O_RDONLY);
    if (-1 == fd)
    {
        printf("Can't open file");
        close(fd);
        exit(1);
    }

    // char buffer[256];
    // int read_status, write_status;
    // while (0 < (read_status = read(fd, buffer, 256)))
    // {
    //     if (-1 == (write_status = write(1, buffer, read_status)))
    //     {
    //         printf("Can't write to stdout");
    //         close(fd);
    //         exit(2);
    //     }
    // }
    char buffer;
    int write_status, read_status;
    while(1 == (read_status = read(fd, &buffer, 1)))
    {
        char b = buffer;
        if (1 != (write_status = write(1, &b, 1)))
        {
            printf("Can't write to stdout");
            close(fd);
            exit(2);
        }
    }
    if (-1 == read_status)
    {
        printf("Can't read file");
        close(fd);
        exit(3);
    }

    close(fd);
    return 0;
}