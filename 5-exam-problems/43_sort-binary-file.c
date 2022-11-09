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

int sort_file(int fd)
{
    int size = 0;
    if (-1 == lseek(fd, 0, SEEK_SET) ||
        -1 == (size = lseek(fd, 0, SEEK_END)))
        return -1;
    if (-1 == lseek(fd, 0, SEEK_SET))
        return -1;

    char left = 0, right = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (-1 == lseek(fd, i, SEEK_SET) || -1 == read(fd, &left, 1) ||
                -1 == lseek(fd, j, SEEK_SET) || -1 == read(fd, &right, 1))
                return -1;
            if (right < left)
            {
                if (-1 == lseek(fd, j, SEEK_SET) || -1 == write(fd, &left, 1) ||
                    -1 == lseek(fd, i, SEEK_SET) || -1 == write(fd, &right, 1))
                    return -1;
            }
        }
    }

    return 0;
}

void print_help(const char *program_name)
{
    puts("NAME\n\tsort-binary-file - sorts a binary file\n\n");
    printf("SYNOPSYS\n\t%s [FILE NAME]\n\n", program_name);
    puts("DESCRIPTION\n\tAvailable options:\n\t --help\n\t\t prints help menu\n\n");
}

int main(int argc, char const *argv[])
{
    if (1 == argc)
    {
        errx(EXIT_FAILURE, "Not enough arguments\n");
    }
    if (2 < argc)
    {
        errx(EXIT_FAILURE, "Too many arguments\n");
    }

    if (0 == strcmp_(argv[1], "--help"))
    {
        print_help(argv[1]);
    }

    int fd = open(argv[1], O_RDWR);
    if (-1 == fd)
    {
        err(EXIT_FAILURE, NULL);
    }

    if (-1 == sort_file(fd))
    {
        close(fd);
        err(EXIT_FAILURE, NULL);
    }

    close(fd);
    return 0;
}
