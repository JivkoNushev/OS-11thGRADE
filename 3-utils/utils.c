#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <sys/types.h>
#include <unistd.h>
#include "utils.h"

int strlen_(const char *str)
{
    unsigned int len = 0;
    if (NULL == str)
    {
        return 0;
    }
    for (const char *l = str; '\0' != *l; l++, len++)
        ;
    return len;
}

int strcmp_(const char *left, const char *right)
{
    for (const char *start_one = left, *start_two = right; '\0' != *start_one || '\0' != *start_two; start_one++, start_two++)
    {
        if ('\0' == *start_one)
        {
            return -1;
        }
        else if ('\0' == *start_two)
        {
            return 1;
        }
        if (*start_one != *start_two)
        {
            return *start_one < *start_two ? -1 : 1;
        }
    }

    return 0;
}

char *strcat_(char *one, char *two)
{
    char *res = malloc(sizeof(char) * (strlen_(one) + strlen_(two) + 1));
    if (NULL == res)
    {
        puts("Couldn't malloc res");
        exit(1);
    }

    unsigned int len = 0;
    for (char *l = one; '\0' != *l; l++)
        res[len++] = *l;
    for (char *l = two; '\0' != *l; l++)
        res[len++] = *l;
    res[len] = '\0';

    return res;
}

void swap_int_(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swap_(void *a, void *b, int size)
{
    char temp;
    for (int i = 0; i < size; i++)
    {
        temp = ((char *)a)[i];
        ((char *)a)[i] = ((char *)b)[i];
        ((char *)b)[i] = temp;
    }
}

char *strdub_(const char *string)
{
    char *res = malloc(sizeof(char) * (strlen_(string) + 1));
    if (NULL == res)
    {
        puts("Couldn't malloc res");
        exit(1);
    }
    char *r = res;
    for (const char *l = string; '\0' != *l; l++, r++)
        *r = *l;
    res[strlen_(string)] = '\0';

    return res;
}

char *read_line_(int fd, int mode, ...)
{
    int end = 0;

    va_list arg;
    va_start(arg, mode);
    end = va_arg(arg, int);
    va_end(arg);

    if (0 == end)
    {
        end = count_bytes(fd) - 1;
    }

    if (0 == count_bytes(fd))
    {
        end = 0;
    }
    else if (0 > end)
    {
        end = count_bytes(fd) + end;
    }

    if (1 != mode && 0 != mode)
    {
        puts("Invalid mode for read_line\n");
        exit(10);
    }

    char buffer[256];
    unsigned int line_size = -1;
    int curr_off = lseek(fd, 0, SEEK_CUR);
    if (-1 == curr_off)
    {
        puts("Couldn't lseek fd\n");
        exit(5);
    }

    int i = 0;
    unsigned int buffer_c = 0;

    int read_status = 0;
    int e = end;
    if (1 == mode) // read_backwards
    {
        end = end - sizeof buffer;
        if (0 > end)
            end = 0;

        if (-1 == lseek(fd, end, SEEK_SET))
        {
            puts("Couldn't lseek fd\n");
            exit(7);
        }

        while (0 < (read_status = read(fd, buffer, sizeof buffer)))
        {
            for (i = read_status - 1; i > -1; i--)
            {
                if('\n' == buffer[i])
                {
                    i = read_status - i;
                    break;
                }
            }
            if (i != read_status)
            {
                if (-1 == lseek(fd, e - i, SEEK_SET))
                {
                    return NULL;
                }
                break;
            }

            if (sizeof buffer == read_status)
            {
                buffer_c++;
            }


            end = end - sizeof buffer;
            if (0 > end)
                end = 0;
            if (-1 == lseek(fd, end, SEEK_SET))
            {
                puts("Couldn't lseek fd\n");
                exit(7);
            }
            
        }

        if (-1 == read_status)
        {
            puts("Couldn't read file\n");
            exit(8);
        }

        curr_off = i + 1;
    }
    else
    {
        while (0 < (read_status = read(fd, buffer, sizeof buffer)))
        {
            for (i = 0; i < read_status; i++)
            {
                if ('\n' == buffer[i])
                {
                    break;
                }
            }
            if (i != read_status)
            {
                break;
            }

            if (sizeof buffer == read_status)
            {
                buffer_c++;
            }
        }
        if (-1 == read_status)
        {
            puts("Couldn't read file\n");
            exit(1);
        }

        if (-1 == lseek(fd, curr_off, SEEK_SET))
        {
            puts("Couldn't lseek fd\n");
            exit(6);
        }

        curr_off += line_size + 1;
    }

    line_size = sizeof buffer * buffer_c + i;
    char *line = (char *)malloc(sizeof(char) * (line_size + 1));
    if (NULL == line)
    {
        puts("Couldn't malloc new line\n");
        exit(2);
    }


    if (line_size != (read_status = read(fd, line, line_size)))
    {
        puts("Couldn't read whole line\n");
        exit(3);
    }

    line[line_size] = '\0';

    if (-1 == lseek(fd, curr_off, SEEK_SET))
    {
        puts("Couldn't lseek fd1\n");
        exit(4);
    }
    return line;
}

int count_bytes(int fd)
{
    if (-1 == lseek(fd, 0, SEEK_SET))
    {
        puts("Couldn't lseek fd\n");
        exit(2);
    }

    int bytes_c = 0;
    if (-1 == (bytes_c = lseek(fd, 0, SEEK_END)))
    {
        puts("Couldn't lseek fd\n");
        exit(2);
    }

    if (-1 == lseek(fd, 0, SEEK_SET))
    {
        puts("Couldn't lseek fd\n");
        exit(2);
    }

    return bytes_c;
}

int count_lines(int fd)
{
    int lines_c = 0;
    char buffer[256];
    int read_status = 0;

    if (-1 == lseek(fd, 0, SEEK_SET))
    {
        puts("Couldn't lseek fd\n");
        exit(3);
    }

    int i = 0;
    while (0 < (read_status = read(fd, buffer, sizeof buffer)))
    {
        for (i = 0; i < read_status; i++)
        {
            if ('\n' == buffer[i])
            {
                lines_c++;
            }
        }
        if (i != sizeof buffer)
        {
            if ('\n' != buffer[i - 1])
            {
                lines_c++;
            }
        }
    }
    if (-1 == read_status)
    {
        puts("Could't read file\n");
        exit(1);
    }
    if (-1 == lseek(fd, 0, SEEK_SET))
    {
        puts("Couldn't lseek fd\n");
        exit(2);
    }
    return lines_c;
}

unsigned int abs_(int n)
{
    return (n < 0) * -n + (n > 0) * n;
}

void print_file(int fd)
{
    char buffer[256];

    int read_status = 0;
    while (0 < (read_status = read(fd, buffer, sizeof buffer)))
    {
        if (-1 == write(0, buffer, read_status))
        {
            puts("Couldn't write to stdout\n");
            exit(1);
        }
    }
    if (-1 == read_status)
    {
        puts("Couldn't read to file\n");
        exit(2);
    }
}