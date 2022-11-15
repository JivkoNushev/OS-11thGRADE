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

struct set
{
    uint32_t start, len;
};

int write_intervals(int fd1, int fd2, int fd3)
{
    struct set set_buffer;
    uint32_t read_buffer[256] = {0};

    int read_status = 0, set_status = 0;
    while(sizeof(struct set) == (set_status = read(fd1, &set_buffer, sizeof set_buffer)))
    {
        printf("NEW SET\nstart = %u, len = %u\n", set_buffer.start, set_buffer.len);

        if(-1 == lseek(fd2, sizeof set_buffer.start * (set_buffer.start - 1), SEEK_SET))
        {
            return -1;
        }
        while (0 < (read_status = read(fd2, read_buffer, sizeof read_buffer)))
        {
            printf("BUFFER NUMBERS\n");
            for (int i = 0; i < read_status; i++)
            {
                printf("%d, ", read_buffer[i]);
            }
            
            if(set_buffer.len <= read_status)
            {
                if(-1 == write(fd3, read_buffer, sizeof *read_buffer * set_buffer.len))
                {
                    return -1;
                }
                break;
            }
            
            if(-1 == write(fd3, read_buffer, read_status))
            {
                return -1;
            }
            set_buffer.len -= read_status;
        }
        if(-1 == read_status)
        {
            return -1;
        }
    }
    if(-1 == set_status)
    {
        return -1;
    }
    if(0 != set_status && sizeof(struct set) != set_status)
    {
        close(fd1);
        close(fd2);
        close(fd3);
        errx(EXIT_FAILURE, "Non valid set of numbers\nset_status = %d\n struct set = %ld", set_status, sizeof(struct set));
    }

    if(-1 == lseek(fd1, 0, SEEK_SET) || -1 == lseek(fd2, 0, SEEK_SET) || -1 == lseek(fd3, 0, SEEK_SET))
    {
        return -1;
    }
    
    return 0;
}

int add_set(char *file_name, struct set Set)
{
    int fd = open(file_name, O_WRONLY | O_APPEND);
    if(-1 == fd)
    {
        close(fd);
        return -1;
    }

    if(-1 == write(fd, &Set, sizeof(struct set)))
    {
        close(fd);
        return -1;
    }

    if(-1 == close(fd))
    {
        return -1;
    }
    return 0;
}

int add_numbers(char *file_name, uint32_t *numbers, size_t size)
{
    int fd = open(file_name, O_WRONLY | O_APPEND);
    if(-1 == fd)
    {
        close(fd);
        return -1;
    }

    if(-1 == write(fd, numbers, sizeof *numbers * size))
    {
        close(fd);
        return -1;
    }

    if(-1 == close(fd))
    {
        return -1;
    }
    return 0;
}


int main(int argc, char const *argv[])
{
    // struct set set2 = {21,5};
    // uint32_t nums1[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

    // add_set("f_uint32_1", set2);
    // add_numbers("f_uint32_2", nums1, 20);
    int fd1 = 0, fd2 = 0, fd3 = 0;

    int exit_status = 0;
    do
    {
        fd1 = open("f_uint32_1", O_RDONLY);
        if(-1 == fd1)
        {
            exit_status = EXIT_FAILURE;
            break;
        }
        fd2 = open("f_uint32_2", O_RDONLY);
        if(-1 == fd2)
        {
            exit_status = EXIT_FAILURE;
            break;
        }

        if(-1 == (fd3 = open("f_uint32_3", O_WRONLY | O_APPEND)))
        {
            errno = 0;
            if(-1 == (fd3 = open("f_uint32_3", O_WRONLY | O_APPEND | O_CREAT)))
            {
                exit_status = EXIT_FAILURE;
                break;
            }
        }

        if(-1 == write_intervals(fd1, fd2, fd3))
        {
            exit_status = EXIT_FAILURE;
            break;
        }
    } while (0);
    if(-1 == close(fd1) || -1 == close(fd2) || -1 == close(fd3))
    {
        exit_status = EXIT_FAILURE;
    }
    err(exit_status, NULL);
    return 0;
}
