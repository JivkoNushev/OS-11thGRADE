#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

#include "head.h"
#include "../3-utils/utils.h"

int head(int fd, int argc, const char **argv)
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
    
    int end = lseek(fd, 0, SEEK_END), offset = lseek(fd, 0, SEEK_SET);
    char *line = 0;
    for(int i = 0; i < lines_count; i++)
    {
        if(lseek(fd, 0, SEEK_CUR) > end)
        {
            break;
        }
        line = read_line_(fd);
        printf("%s\n", line);

        offset += strlen_(line) + 1;
        lseek(fd, offset, SEEK_SET);

        free(line);
    }

    return 0;
}