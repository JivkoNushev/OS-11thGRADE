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

    for(int i = 2; i < argc; i++)
    {
        if (0 == strcmp_("-n", argv[i])) 
        {
            if(i+1 < argc)
            {
                if(0 == (lines_count = atoi(argv[++i])))
                {
                    puts("Number of lines must be an integer value and not 0\n");
                    exit(2);
                }
            }
            else
            {
                puts("Option requires a non 0 integer argument\n");
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

    unsigned int start_from_line = lines_count - lines_c;
    start_from_line = start_from_line > 0 ? 0 : start_from_line;

    for (size_t i = 0; i < start_from_line; i++)
    {
        if (NULL == read_line_(fd))
        {
            puts("Couldn't read line\n");
            exit(5);
        }
    }
    print_file(fd);

    return 0;
}