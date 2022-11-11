#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

#include "head.h"
#include "../3-utils/utils.h"
#include "../3-utils/utils.c"

int tail(int fd, int argc, const char **argv)
{
    if (argc < 2)
    {
        puts("Not enough arguments\n");
        exit(1);
    }

    int lines_count = 10;
    for (int i = 2; i < argc; i++)
    {
        if (0 == strcmp_("-n", argv[i]))
        {
            if (i + 1 < argc)
            {
                if (0 == (lines_count = atoi(argv[++i])))
                {
                    if (0 != strcmp_("0", argv[i]))
                    {
                        printf("Invalid number of lines: '%s'\n", argv[i]);
                        exit(2);
                    }
                }
            }
            else
            {
                puts("Option requires an integer argument\n");
                exit(3);
            }
        }
        else
        {
            printf("Argument %s is unknown\n", argv[i]);
            exit(4);
        }
    }

    size_t line_counter = 0;
    char block[256];

    int index = lseek(fd, 0, SEEK_END);
    if (-1 == index)
    {
        return -1;
    }
    index -= sizeof block;
    if (0 > index)
        index = 0;

    if (-1 == lseek(fd, index, SEEK_SET))
    {
        return -1;
    }

    size_t i = 0, blocks_count = 0;
    int read_status = 0;
    while (0 < (read_status = read(fd, block, sizeof block)))
    {
        puts("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAaa");
        if (read_status == sizeof block)
        {
            blocks_count++;
        }
        else
        {
            for(int i = read_status; i < sizeof block; i++)
                block[i] = 0;
        }
        printf("%s", block);
        for (i = read_status - 1; - 1 < read_status; i--)
        {
            if ('\n' == block[i])
            {
                printf("%c\n", block[i + 1]);
                line_counter++;
            }
            if (line_counter == lines_count)
            {
                break;
            }
        }
        if (line_counter == lines_count)
        {
            i = read_status - i - 1;
            break;
        }

        index -= sizeof block;
        if (0 > index)
            index = 0;

        if (-1 == lseek(fd, index, SEEK_SET))
        {
            return -1;
        }
    }
    if (-1 == read_status)
    {
        return -1;
    }

    if (line_counter != lines_count)
    {
        if (-1 == lseek(fd, 0, SEEK_SET))
        {
            return -1;
        }
    }
    else
    {
        if (-1 == lseek(fd, -(i + blocks_count * sizeof block), SEEK_END))
        {
            return -1;
        }
    }

    print_file(fd);
    return 0;
}

int main(int argc, char const *argv[])
{
    int fd = open(argv[1], O_RDONLY);
    if (-1 == fd)
    {
        err(EXIT_FAILURE, NULL);
    }

    tail(fd, argc, argv);

    close(fd);
    return 0;
}
