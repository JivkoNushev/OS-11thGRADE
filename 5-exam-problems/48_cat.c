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

void print_help(const char *program_name)
{
    puts("NAME\n\tcat - concatenate files and print on the standard output\n\n");
    printf("SYNOPSYS\n\t%s [OPTION]... [FILE]...\n\n", program_name);
    puts("DESCRIPTION\n\tConcatenate FILE(s) to standard output.\n\nAvailable options:\n\t \
    --help\n\t\t prints help menu\n\n \
    -n\n\t\t number all output lines\n\n");
}

int cat_file(const char *file_name, int options, ...)
{
    size_t line_number = 0;
    if(2 & options)
    {
        va_list l;
        va_start(l, options);
        line_number = va_arg(l, size_t);
        va_end(l);
    }

    int fd = open(file_name, O_RDONLY);
    if(-1 == fd)
    {
        return -1;
    }
    if(2 & options)
    {
        printf("\t %ld ", line_number++);
        if(-1 == read_line(fd))
        {
            return -1;
        }
    }
    else if(-1 == print_file(fd))
    {
        return -1;
    }

    if(-1 == close(fd))
    {
        return -1;
    }

    return line_number;
}

int main(int argc, char const *argv[])
{
    // --help = 1
    // -n = 2
    uint8_t options = 0;
    size_t line_number = 1;

    int exit_status = EXIT_SUCCESS;
    do
    {
        for (size_t i = 1; i < argc; i++)
        {
            if(0 == strcmp_(argv[i], "--help"))
            {
                options |= 1;
                break;
            }
            else if(0 == strcmp_(argv[i], "-n"))
            {
                if(1 == i)
                {
                    options |= 2;
                    break;
                }
            }
            else if(0 == strcmp_(argv[i], "-"))
            {
                if(2 & options)
                {
                    printf("\t %ld ", line_number++);
                    if(-1 == read_line(STDIN_FILENO))
                    {
                        exit_status = EXIT_FAILURE;
                        break;
                    }
                }
                else if(-1 == print_file(STDIN_FILENO))
                {
                    exit_status = EXIT_FAILURE;
                    break;
                }
            }
            else
            {
                if(-1 == (line_number = cat_file(argv[i], options)))
                {
                    exit_status = EXIT_FAILURE;
                    break;
                }
            }
        }
        
        if(1 & options)
        {
            print_help(argv[0]);
            break;
        }
        if(1 == argc)
        {
            if(-1 == print_file(STDIN_FILENO))
            {
                exit_status = EXIT_FAILURE;
                break;
            }
        }
    } while (0);
    
    if(EXIT_FAILURE == exit_status)
    {
        err(exit_status, NULL);
    }
    return 0;
}
