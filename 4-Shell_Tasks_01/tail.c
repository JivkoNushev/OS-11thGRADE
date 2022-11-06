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

    unsigned int lines_c = count_lines(fd);

    int start_from_line = abs_(lines_count) - lines_c;
    start_from_line = start_from_line > 0 ? 0 : -start_from_line;

    int offset = 0;
    for (size_t i = 0; i < start_from_line; i++)
    {
        offset += strlen_(read_line_(fd)) + 1;
    }
    print_file(fd);
    return 0;
}