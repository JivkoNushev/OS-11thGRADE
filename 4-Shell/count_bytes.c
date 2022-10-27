#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int count_bytes(int fd)
{
    int blocks_c = 0;
    int bytes_c = 0;
    char buffer[256];
    int read_status = 0;

    while(sizeof buffer == (read_status = read(fd, buffer, sizeof buffer)))
    {
        blocks_c++;
    }
    if(-1 == read_status)
    {
        puts("Could't read file\n");
        exit(1);
    }
    if(sizeof buffer != read_status)
    {
        bytes_c += read_status;
    }
    bytes_c += sizeof buffer * blocks_c; 

    return bytes_c;
}