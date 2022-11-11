#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>


#include "utils.h"
#include "test.h"

int main(int argc, char const *argv[])
{
    // if(0 == test_read_line_())
    // {
    //     puts("TEST SUCCEEDED\n");
    // }
    // else
    // {
    //     puts("TEST FAILED\n");
    //     exit(1);
    // }
    
    int fd = open("file", O_RDONLY);
    if(-1 == fd)
    {
        err(EXIT_FAILURE, NULL);
    } 

    char *line = read_line_(fd, LINE_FORWARD, 3);

    printf("%s\n", line);

    close(fd);
    return 0;
}