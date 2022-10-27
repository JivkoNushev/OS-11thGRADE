#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int count_lines(int fd)
{
    int lines_c = 0;
    char buffer[256];
    int read_status = 0;

    while(0 != (read_status = read(fd, buffer, sizeof buffer)))
    {
        for(int i = 0; i < read_status; i++)
        {
            if('\n' == buffer[i])
            {
                lines_c++;
            }
        }
    }
    if(-1 == read_status)
    {
        puts("Could't read file\n");
        exit(1);
    }
    return lines_c;
}