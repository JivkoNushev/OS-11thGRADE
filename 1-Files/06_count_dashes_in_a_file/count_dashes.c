#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int fd = open("dashes", O_RDONLY);
    if (-1 == fd)
    {
        printf("Can't open file");
        close(fd);
        exit(1);
    }

    int dashes_count = 0;
    int read_status = 0;
    char chunk[4];
    while (sizeof chunk == (read_status = read(fd, chunk, sizeof chunk)))
    {
        for (int i = 0; i < sizeof chunk; i++)
        {
            if (chunk[i] == '-')
            {
                dashes_count++;
            }
        }
    }
    if (-1 == read_status)
    {
        printf("ALOOO");
        printf("Can't read from file");
        close(fd);
        exit(2);
    }
    for (int i = 0; i < read_status; i++)
    {
        if (chunk[i] == '-')
        {
            dashes_count++;
        }
    }

    if ( -1 == fcntl(fd, F_SETFD, O_RDWR))
        printf("ALOOO");

    printf("Dashes count: %d", dashes_count);

    return 0;
}
