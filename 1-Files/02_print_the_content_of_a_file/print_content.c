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
    
    int len = lseek(fd, 0, SEEK_END);
    if (-1 == len)
    {
        printf("Can't lseek file to end");
        close(fd);
        exit(2);
    }
    if (-1 == lseek(fd, 0, SEEK_SET))
    {
        printf("Can't lseek file to start");
        close(fd);
        exit(2);
    }

    char *buffer = (char*)malloc(sizeof(char) * len);

    int read_status = read(fd, buffer, len);
    if (-1 == read_status)
    {
        printf("Can't read file");
        close(fd);
        exit(3);
    }

    int write_status = write(1, buffer, len);
    if(-1 == write_status)
    {
        printf("Can't write to stdout");
        close(fd);
        exit(4);
    }

    close(fd);
    return 0;
}