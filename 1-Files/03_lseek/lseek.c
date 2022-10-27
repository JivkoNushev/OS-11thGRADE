#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int fd = open("testfile", O_RDWR);
    if (-1 == fd)
    {
        printf("Can't open file");
        close(fd);
        exit(1);
    }

    char buffer[5];

    int possition = lseek(fd, 0, SEEK_END);
    printf("End of file possition: %d\n", possition);

    possition = lseek(fd, 15, SEEK_END);
    printf("15 after End of file possition: %d\n", possition);
    possition = lseek(fd, -9, SEEK_CUR);
    printf("-9 after 15 after End of file possition: %d\n", possition);
    possition = lseek(fd, -3, SEEK_END);
    printf("-3 after after End of file possition: %d\n", possition);


// new fd to the same file
    int fd2 = open("testfile", O_RDWR);
    int new_pos = lseek(fd2, 0, SEEK_CUR);

    read(fd, buffer, 1);
    possition = lseek(fd, 0, SEEK_CUR);
    new_pos = lseek(fd2, 0, SEEK_CUR);

    read(fd2, buffer, 3);
    possition = lseek(fd, 0, SEEK_CUR);
    new_pos = lseek(fd2, 0, SEEK_CUR);
    printf("\n%d %d", possition, new_pos);

    return 0;
}
