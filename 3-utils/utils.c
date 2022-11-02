#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>
#include "utils.h"

int strlen_(const char *str)
{
    unsigned int len = 0;

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

char *read_line_(int fd)
{
    int read_status = 0;
    char buffer[256];
    int i = 0;
    int block_count = 0;
    while (sizeof buffer == (read_status = read(fd, buffer, sizeof buffer)))
    {
        block_count++;
        for (; i < sizeof buffer; i++)
        {
            if ('\n' == buffer[i])
                break;
        }
        if (sizeof buffer != i)
            break;
        i = 0;
    }
    if (-1 == read_status)
    {
        puts("Couldn't read file !");
        exit(2);
    }
    if (sizeof buffer != read_status)
    {
        for (i = 0; i < read_status; i++)
        {
            if ('\n' == buffer[i])
                break;
        }
        if (read_status == i)
            i = 0;
    }
    if (0 != i)
    {
        char *res = (char *)malloc(sizeof(char) * (i + 1));
        if (NULL == res)
        {
            puts("Couldn't malloc res for read_line");
            exit(1);
        }
        *res = '\0';
        buffer[i] = '\0';
        char *temp = res;

        res = strcat_(res, buffer);
        free(temp);
        return res;
    }

    int res_s = sizeof buffer * block_count;
    char *res = (char *)malloc(sizeof(char) * (res_s + 1));
    if (NULL == res)
    {
        puts("Couldn't malloc res for read_line");
        exit(1);
    }

    if (-1 == (read_status = read(fd, res, res_s)))
    {
        puts("Couldn't read file");
        exit(2);
    }
    res[res_s] = '\0';
    if (-1 == (read_status = read(fd, buffer, sizeof buffer)))
    {
        puts("Couldn't read file");
        exit(2);
    }
    buffer[read_status] = '\0';
    res = realloc(res, sizeof(char) * (res_s + read_status + 1));
    if (NULL == res)
    {
        puts("Couldn't realloc res for read_line");
        exit(1);
    }
    char *temp = res;
    res = strcat_(res, buffer);
    free(temp);

    return res;
}

int count_bytes(int fd)
{
    int blocks_c = 0;
    int bytes_c = 0;
    char buffer[256];
    int read_status = 0;

    while (sizeof buffer == (read_status = read(fd, buffer, sizeof buffer)))
    {
        blocks_c++;
    }
    if (-1 == read_status)
    {
        puts("Could't read file\n");
        exit(1);
    }
    if (sizeof buffer != read_status)
    {
        bytes_c += read_status;
    }
    bytes_c += sizeof buffer * blocks_c;

    return bytes_c;
}

int count_lines(int fd)
{
    int lines_c = 0;
    char buffer[256];
    int read_status = 0;

    while (0 != (read_status = read(fd, buffer, sizeof buffer)))
    {
        for (int i = 0; i < read_status; i++)
        {
            if ('\n' == buffer[i])
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
    return lines_c;
}