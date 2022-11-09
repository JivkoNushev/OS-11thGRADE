#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

#include "head.h"
#include "../3-utils/utils.h"

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
                    if(0 != strcmp_("0", argv[i]))
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

    int offset = count_bytes(fd);
    char *line = 0;
    for (size_t i = 0; i < abs_(lines_count); i++)
    {
        line = read_line_(fd, 1, offset);
        offset -= strlen_(line) + 1;
        free(line);
    }
    print_file(fd);
    return 0;
}