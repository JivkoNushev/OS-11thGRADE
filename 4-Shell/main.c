#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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

int wc(int fd, int argc, const char **argv);

int main(int argc, const char *argv[])
{
    int fd = open("file", O_RDONLY);
    if(-1 == fd)
    {
        puts("Couldn't open file\n");
        exit(1);
    }

    wc(fd, argc, argv);

    close(fd);
    return 0;
}
