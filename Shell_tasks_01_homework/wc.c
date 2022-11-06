#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

#include "../3-utils/utils.h"


// counts last line without \n too
int wc(int fd, int argc, const char **argv)
{
    int args = 0;

    if(argc < 2)
    {
        puts("Not enough arguments\n");
        exit(1);
    }
    if (argc == 2)
    {
        args = 3;
    }
    for(int it = 2; it < argc; it++)
    {
        if(0 == strcmp_("-l", argv[it]))
        {
            args |= 1;
        }
        else if(0 == strcmp_("-c", argv[it]))
        {
            args |= 2;
        }
        else
        {
            printf("Invalid option %s\n", argv[it]);
            exit(-1);
        }
    }
    if(args & 1)
    {
        printf("lines:%d\n", count_lines(fd));
        lseek(fd, 0, SEEK_SET);
    }
        
    if(args & 2)
    {
        printf("bytes:%d\n", count_bytes(fd));
        lseek(fd, 0, SEEK_SET);
    }
    return 1;
}