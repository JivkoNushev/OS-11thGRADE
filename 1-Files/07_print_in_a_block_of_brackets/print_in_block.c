#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int fd = open("blocks", O_RDONLY);
    if (-1 == fd)
    {
        printf("Can't open file");
        close(fd);
        exit(1);
    }

    int read_status = 0;
    char chunk[4];
    int in_block = 0;
    while (sizeof chunk == (read_status = read(fd, chunk, sizeof chunk)))
    {
        for (int i = 0; i < sizeof chunk; i++)
        {
            if ('{' == chunk[i])
            {
                in_block++;
            }
            if (0 < in_block)
            {
                printf("%c", chunk[i]);
            }
            if ('}' == chunk[i])
            {
                if (in_block > 0)
                    in_block--;
            }
        }
    }
    if (-1 == read_status)
    {
        printf("Can't read file");
        close(fd);
        exit(2);
    }
    for (int i = 0; i < read_status; i++)
    {
        if ('{' == chunk[i])
        {
            in_block++;
        }
        if (0 < in_block)
        {
            printf("%c", chunk[i]);
        }
        if ('}' == chunk[i])
        {
            if (in_block > 0)
                in_block--;
        }
    }

    return 0;
}
