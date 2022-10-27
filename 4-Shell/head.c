#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

int strcmp_(const char *left, const char *right);
char *read_line_(int fd);

int head(int fd, int argc, const char **argv)
{
    int args = 0;

    if (argc != 2 || argc != 4)
    {
        puts("Wrong arguments\n");
        exit(1);
    }

    if(argc == 2)
    {

    }
    else
    {

    }

    for (int it = argc - 2; it > 0; it--)
    {
        if (0 == strcmp_("-l", argv[it + 1]))
        {
            args |= 1;
        }
        else if (0 == strcmp_("-c", argv[it + 1]))
        {
            args |= 2;
        }
    }
    if (args & 1)
    {
        printf("%d\n", count_lines(fd));
        lseek(fd, 0, SEEK_SET);
    }

    if (args & 2)
    {
        printf("%d\n", count_bytes(fd));
        lseek(fd, 0, SEEK_SET);
    }
}