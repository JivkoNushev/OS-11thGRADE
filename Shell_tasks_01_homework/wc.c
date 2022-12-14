#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "utils.h"

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
            exit(2);
        }
    }
    if(args & 1)
    {
        printf("lines:%d\n", count_lines(fd));
    }
        
    if(args & 2)
    {
        printf("bytes:%d\n", count_bytes(fd));
    }
    return 0;
}

int main(int argc, const char *argv[])
{
    int fd = open(argv[1], O_RDONLY);
    if(0 == fd)
    {
        puts("Couldn't open file");
        exit(1);
    }
    wc(fd, argc, argv);

    return 0;
}