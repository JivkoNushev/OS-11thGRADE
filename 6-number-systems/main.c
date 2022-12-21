#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <errno.h>
#include <err.h>

#include "../3-utils/utils.h"
#include "../3-utils/utils.c"

int print_number(int fd, uint32_t number, uint32_t system, uint32_t depth)
{
    if(0 == depth)
    {
        return 0;
    }
    
    if (-1 == print_number(fd, number / system, system, depth-1))
    {
        return -1;
    }
    char n = '0' + number%system;
    printf("%c", n);
    if(1 != write(fd, &n, 1))
    {
        return -1;
    }
    return number;
}

int print_number_in_number_system_to_file(int fd, uint32_t number, uint32_t system, uint32_t size)
{
    if(0 == number)
    {
        char n[] = "0000000000000";
        printf("%s", n);
    
        if(13 != write(fd, n, size))
        {
            return -1;
        }
    }
    else
    {
        if(-1 == print_number(fd, number, system, size))
        {
            return -1;
        }
    }
    char new_line = '\n';

    if (1 != write(fd, &new_line, 1))
    {
        close(fd);
        err(EXIT_FAILURE, NULL);
    }
    puts("");

    return 0;
}

int filter_file(int fd_from, int fd_to, uint32_t size)
{
    int a = 7;
    char number_buffer[size];

    uint32_t counter[3] = {0,0,0};

    int read_status = 0;
    while (size == (read_status = read(fd_from, number_buffer, size)))
    {
        for(int i = 0; i < size; i++)
        {
            switch(atoi(number_buffer + i))
            {
                case 2:
                    counter[2]++;
                    break;
                case 1:
                    counter[1]++;
                    break;
                case 0:
                    counter[0]++;
                    break;
                default:
                    break;
            }   
        }
        if(counter[0] == 3 && counter[1] == 4 && counter[2] == 5)
        {
            if(size != write(fd_to, number_buffer, size))
            {
                return -1;
            }
        }
    }
    if(read_status != size && read_status != 0)
    {
        return -1;
    }
    
}

int main(int argc, char const *argv[])
{
    uint32_t number = 1589764;
    uint32_t system = 3;
    uint32_t size = 13;

    number = 150000;

    int exit_status = EXIT_SUCCESS;
    int fd = open("file", O_RDWR | O_TRUNC | O_APPEND);
    if(-1 == fd)
    {
        err(EXIT_FAILURE, NULL);
    }

    for(uint32_t i = 0; i < number; i++)
    {
        if(-1 == print_number_in_number_system_to_file(fd, i, system, size))      
        {
            close(fd);
            err(EXIT_FAILURE, NULL);
        }  
    }

    if(-1 == lseek(fd, 0, SEEK_SET))
    {
        close(fd);
        err(EXIT_FAILURE, NULL);
    }

    int fd_filtered = open("file_filtered", O_WRONLY | O_TRUNC | O_APPEND);
    if(-1 == fd_filtered)
    {
        close(fd);
        err(EXIT_FAILURE, NULL);
    }
    
    if(-1 == filter_file(fd, fd_filtered, size))
    {
        close(fd);
        close(fd_filtered);
        err(EXIT_FAILURE, NULL);
    }

    return 0;
}
