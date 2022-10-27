#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void first_task()
{
    int fd = open("file", O_RDONLY);
    if (-1 == fd)
    {
        printf("Can't open file");
        exit(1);
    }

    char chunk[2];
    int read_status = 0;
    int write_status = 0;
    while (sizeof chunk == (read_status = read(fd, chunk, sizeof chunk)))
    {
        if (-1 == (write_status = write(1, chunk, sizeof chunk)))
        {
            printf("Can't write to stdout");
            close(fd);
            exit(2);
        }
    }
    if (-1 == read_status)
    {
        printf("Can't read file");
        close(fd);
        exit(3);
    }
    if (0 < read_status)
    {
        if (-1 == (write_status = write(1, chunk, read_status)))
        {
            printf("Can't write to stdout");
            close(fd);
            exit(4);
        }
    }

    close(fd);
    exit(0);
}

void second_task()
{
    int first_fd = open("a.txt", O_RDONLY);
    int second_fd = open("b.txt", O_WRONLY | O_TRUNC | O_APPEND);
    if(-1 == first_fd || -1 == second_fd)
    {
        printf("Can't open file");
        close(first_fd);
        close(second_fd);
        exit(1);
    }

    char chunk[2];
    int read_status = 0;
    int write_status = 0;
    while (sizeof chunk == (read_status = read(first_fd, chunk, sizeof chunk)))
    {
        if (-1 == (write_status = write(second_fd, chunk, sizeof chunk)))
        {
            printf("Can't write to stdout");
            close(first_fd);
            close(second_fd);
            exit(2);
        }
    }
    if (-1 == read_status)
    {
        printf("Can't read file");
        close(first_fd);
    close(second_fd);
        exit(3);
    }
    if (0 < read_status)
    {
        if (-1 == (write_status = write(second_fd, chunk, read_status)))
        {
            printf("Can't write to stdout");
            close(first_fd);
            close(second_fd);
            exit(4);
        }
    }

    close(first_fd);
    close(second_fd);
    exit(0);
}

void swap(char *a, char *b)
{
    char temp = *a;
    *a = *b;
    *b = temp;
}

void sort_chunk(char *chunk, unsigned int size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = i + 1; j < size; j++)
        {
            if(chunk[j] < chunk[i])
            {
                swap(chunk + i, chunk + j);
            }
        }
    }
}

void _sort()
{   
    int fd = open("input", O_RDWR);
    int size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    char left;
    char right;

    int read_status = 0, write_status = 0;

    for(int i = 0; i < size; i++)
    {
        for(int j = i + 1; j < size; j++)
        {
            lseek(fd, i, SEEK_SET);
            if(-1 == (read_status = read(fd, &left, 1)))
            {
                exit(1);
            }
            lseek(fd, j, SEEK_SET);
            if(-1 == (read_status = read(fd, &right, 1)))
            {
                exit(1);
            }
            if(right < left)
            {
                lseek(fd, j, SEEK_SET);
                if(-1 ==(write_status = write(fd, &left, 1)))
                {
                    exit(1);
                }
                lseek(fd, i, SEEK_SET);
                if(-1 ==(write_status = write(fd, &right, 1)))
                {
                    exit(1);
                }
            }
        }
    }

}

void third_task()
{
    int first_fd = open("input", O_RDONLY);
    if(-1 == first_fd)
    {
        printf("Can't open file");
        exit(1);
    }

    int size = lseek(first_fd, 0, SEEK_END);
    lseek(first_fd, 0, SEEK_SET);

    char *chunk = (char*)malloc(sizeof(char) * size);
    if(0 == chunk)
    {
        printf("Can't malloc");
        close(first_fd);
        exit(5);
    }
    int read_status = 0;
    int write_status = 0;
    if (size == (read_status = read(first_fd, chunk, size)))
    {
        sort_chunk(chunk, size);
        close(first_fd);
        first_fd = open("input", O_WRONLY | O_TRUNC);
        if (-1 == (write_status = write(first_fd, chunk, size)))
        {
            printf("Can't write to stdout");
            close(first_fd);
            exit(2);
        }
    }
    if (-1 == read_status)
    {
        printf("Can't read file");
        close(first_fd);
        exit(3);
    }

    close(first_fd);

    exit(0);
}

int main(int argc, char const *argv[])
{
    // first_task();
    // second_task();
    // third_task();
    _sort();
    return 0;
}
