#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "head.h"
#include "utils.h"

int head(int fd, int argc, const char **argv)
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

    if (lines_count < 0)
    {
        int lines_c = count_lines(fd);
        lines_count += lines_c;
    }

    char *line = 0;
    for (int i = 0; i < lines_count; i++)
    {
        line = read_line_(fd);
        printf("%s\n", line);
        free(line);
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    int fd = open(argv[1], O_RDONLY);
    if(0 == fd)
    {
        puts("Couldn't open file");
        exit(1);
    }
    head(fd, argc, argv);

    return 0;
}
