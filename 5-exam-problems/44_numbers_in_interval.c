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

int print_intervals(int fd1, int fd2)
{
    int fd3 = open("file3", O_WRONLY | O_APPEND);
    if (-1 == fd3)
        return -1;

    uint32_t start = 0, length = 0;
    int read_status = 0;
    uint32_t number = 0;
    while (sizeof(uint32_t) == (read_status = read(fd1, &start, sizeof(uint32_t))) &&
           sizeof(uint32_t) == (read_status = read(fd2, &length, sizeof(uint32_t))))
    {
        if(-1 == lseek(fd2, start, SEEK_SET))
        {
            close(fd3);
            return -1;
        }
        for (size_t i = 0; i < length; i++)
        {
            if(sizeof(uint32_t) != read(fd2, &number, sizeof(uint32_t)))
            {
                close(fd3);
                return -1;
            }
            if(sizeof(uint32_t) != write(fd3, &number, sizeof(uint32_t)))
            {
                close(fd3);
                return -1;
            }
        }
    }
    if(-1 == read_status)
    {
        close(fd3);
        return -1;
    }

    close(fd3);
    return 0;
}

int make_test_files(char *file1_name, char *file2_name)
{
    int fd1 = open(file1_name, O_WRONLY);
    if (-1 == fd1)
    {
        err(EXIT_FAILURE, NULL);
    }
    int fd2 = open(file2_name, O_WRONLY);
    if (-1 == fd2)
    {
        close(fd1);
        err(EXIT_FAILURE, NULL);
    }

    uint32_t a = 0, b = 2, c = 5, d = 15;

    write(fd1, &a, sizeof(uint32_t));
    write(fd1, &b, sizeof(uint32_t));

    write(fd2, &c, sizeof(uint32_t));
    write(fd2, &d, sizeof(uint32_t));

    close(fd1);
    close(fd2);
    return 0;
}

int main(int argc, char const *argv[])
{
    //make_test_files("file1", "file2");

    int fd1 = open("file1", O_RDONLY);
    if (-1 == fd1)
    {
        err(EXIT_FAILURE, NULL);
    }
    int fd2 = open("file1", O_RDONLY);
    if (-1 == fd2)
    {
        close(fd1);
        err(EXIT_FAILURE, NULL);
    }

    if (-1 == print_intervals(fd1, fd2))
    {
        close(fd1);
        close(fd2);
        err(EXIT_FAILURE, NULL);
    }

    int fd3 = open("file3", O_RDONLY);
    if(-1 == fd3)
    {
        close(fd1);
        close(fd2);
        err(EXIT_FAILURE, NULL);
    }

    puts("hello");
    uint32_t num = 0;
    while (0 < read(fd3, &num, sizeof(uint32_t)))
    {
        printf("%u, ", num);
    }
    


    close(fd1);
    close(fd2);
    close(fd3);
    return 0;
}
