#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

#include "head.h"
#include "../3-utils/utils.h"

int head(int fd, int argc, const char **argv)
{
    if (argc != 2 && argc != 4)
    {
        puts("Wrong arguments\n");
        exit(1);
    }

    int lines_count = 0;
    if(argc == 2)
    {
        lines_count = 10;
    }
    else
    {
        if(0 == (lines_count = atoi(argv[3])))
        {
            puts("Number of lines must be an integer value\n");
            exit(1);
        }
        
        // segm fault when "-n", argv[2]
        if (0 != strcmp_("-n", argv[2])) 
        {
            printf("Argument %s is unknown\n", argv[2]);
            exit(1);
        }
    }

    int offset = 0;
    char *line = 0;

    for(int i = 0; i < lines_count; i++)
    {
        lseek(fd, offset, SEEK_SET);
        line = read_line_(fd);
        offset += strlen_(line) + 1; 
        lseek(fd, offset, SEEK_SET);

        if (lseek(fd, 0, SEEK_CUR) >= lseek(fd, 0, SEEK_END))
        {
            break;
        }
        printf("%s\n", line);
        free(line);
    }

    return 0;
}