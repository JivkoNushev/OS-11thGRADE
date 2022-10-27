#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int fd = open("numbers", O_WRONLY | O_TRUNC | O_CREAT);
    if (-1 == fd)
    {
        printf("Can't open file");
        close(fd);
        exit(1);
    }

    int numbers[] = {10, 12, 13, 14};
    int numbers_len = sizeof numbers / sizeof (int);

    int write_status = 0;

    for(int i = 0; i < numbers_len; ++i)
    {
        if(sizeof(int) != (write_status = write(fd, &numbers[i], sizeof(int))))
        {
            printf("Can't write to file");
            close(fd);
            exit(2);
        }
    }


    return 0;
}
