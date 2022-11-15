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

int sort_numbers(int fd, size_t size)
{
    int cur_pos = 0;
    uint32_t best = UINT32_MAX, temp = UINT32_MAX;
    for (size_t i = 0; i < size; i += sizeof(uint32_t))
    {
        for (size_t j = i; j < size; j += sizeof(uint32_t))
        {
            if(-1 == (cur_pos = lseek(fd, j, SEEK_SET)))
            {
                return -1;
            }
            if(sizeof(uint32_t) > read(fd, &temp, sizeof(uint32_t)))
            {
                return -1;
            }
            if(temp < best)
            {
                if(UINT32_MAX != best)
                {
                    if(-1 == lseek(fd, j, SEEK_SET))
                    {
                        return -1;
                    }
                    if(sizeof(uint32_t) > write(fd, &best, sizeof(uint32_t)))
                    {
                        return -1;
                    }
                    if(-1 == lseek(fd, i, SEEK_SET))
                    {
                        return -1;
                    }
                    if(sizeof(uint32_t) > write(fd, &temp, sizeof(uint32_t)))
                    {
                        return -1;
                    }
                }
                best = temp;
            }
        }

        best = UINT32_MAX, temp = UINT32_MAX;
    }
    if (-1 == lseek(fd, 0, SEEK_SET))
    {
        return -1;
    }

    return 0;
}

int write_ints_to_file(int fd, uint32_t *intergers, size_t size)
{
    if(sizeof(uint32_t) * size > write(fd, intergers, sizeof(uint32_t) * size))
    {
        return -1;
    }
    
    if(-1 == lseek(fd, 0, SEEK_SET))
    {
        return -1;
    }

    return 0;
}

int read_ints_from_file(int fd, uint32_t *intergers)
{
    size_t bytes = count_bytes(fd);
    if(-1 == bytes)
    {
        return -1;
    }

    if(bytes > read(fd, intergers, bytes))
    {
        return -1;
    }

    if(-1 == lseek(fd, 0, SEEK_SET))
    {
        return -1;
    }
    return 0;
}

int make_file_of_ints(int fd)
{
    uint32_t ints[10] = {1,23,2,0,500,3,69,110,24,31};
    uint32_t ints2[10];

    if(-1 == write_ints_to_file(fd, ints, sizeof ints / sizeof *ints))
    {
        return -1;
    }
    
    if(-1 == read_ints_from_file(fd, ints2))
    {
        return -1;
    }

    for (size_t i = 0; i < sizeof ints / sizeof *ints; i++)
    {
        printf("%d, ", ints2[i]);
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    if(2 != argc)
    {
        errx(EXIT_FAILURE, "Wrong arguments\n");
    }

    int fd = open(argv[1], O_RDWR);
    if(-1 == fd)
    {
        err(EXIT_FAILURE, NULL);
    }

    if (-1 == make_file_of_ints(fd))
    {
        close(fd);
        err(EXIT_FAILURE, NULL);
    }

    size_t bytes = count_bytes(fd);
    if(-1 == bytes)
    {
        close(fd);
        err(EXIT_FAILURE, NULL);
    }

    if(-1 == sort_numbers(fd, bytes))
    {
        close(fd);
        err(EXIT_FAILURE, NULL);
    }
    uint32_t ints[10] = {0};
    if(-1 == read_ints_from_file(fd, ints))
    {
        close(fd);
        err(EXIT_FAILURE, NULL);
    }
    puts("\n");

    for (size_t i = 0; i < 10; i++)
    {
        printf("%d, ", ints[i]);
    }
    puts("\n");

    if(-1 == close(fd))
    {
        err(EXIT_FAILURE, NULL);
    }
    return 0;
}
