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

struct header
{
    uint32_t magic;
    uint8_t header_version;
    uint8_t data_version;
    uint16_t count;
    uint32_t reserved1;
    uint32_t reserved2;
};

struct data_uint8_t
{
    uint16_t offset;
    uint8_t original_byte;
    uint8_t new_byte;
};

struct data_uint16_t
{
    uint32_t offset;
    uint16_t original_byte;
    uint16_t new_byte;
};

int patch_to_uint8_t(int fd_patch, int fd1, int fd2)
{
    struct data_uint8_t difference = {0,0,0};

    int read_status = 0;
    while(0 < (read_status = read(fd_patch, &difference, sizeof(struct data_uint8_t))))
    {
        if(-1 == lseek(fd2, difference.offset, SEEK_SET))
        {
            return -1;
        }
        if(-1 == write(fd2, difference.new_byte, sizeof(uint8_t)))
        {
            return -1;
        }
    }
    if(-1 == read_status)
    {
        return -1;
    }

    return 0;
}

int patch_to_uint16_t(int fd_patch, int fd1, int fd2)
{
    struct data_uint16_t difference = {0,0,0};

    int read_status = 0;
    while(0 < (read_status = read(fd_patch, &difference, sizeof(struct data_uint16_t))))
    {
        if(-1 == lseek(fd2, difference.offset, SEEK_SET))
        {
            return -1;
        }
        if(-1 == write(fd2, difference.new_byte, sizeof(uint16_t)))
        {
            return -1;
        }
    }
    if(-1 == read_status)
    {
        return -1;
    }

    return 0;
}

int patch_file(int fd_patch, int fd1, int fd2)
{
    struct header patch_header = {0,0,0,0,0,0};

    if(sizeof(struct header) != read(fd_patch, &patch_header, sizeof(struct header)))
    {
        return -1;
    }

    if(0x01 != patch_header.header_version)
    {
        return -1;
    }

    if(-1 == cpyfile(fd2, fd1))
    {
        return -1;
    }

    switch (patch_header.data_version)
    {
    case 0x00:
        if(-1 == patch_to_uint8_t(fd_patch, fd1, fd2))
        {
            return -1;
        }
        break;
    case 0x01:
        if(-1 == patch_to_uint16_t(fd_patch, fd1, fd2))
        {
            return -1;
        }
        break;
    
    default:
        return -1;
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    if (argc != 4)
    {
        errx(EXIT_FAILURE, "Not enough parameters\n\t$ %s [patch_file_name.bin] [file_name.bin] [file_name.bin]\n", argv[0]);
    }
    int fd_patch = 0, fd1 = 0, fd2 = 0;

    int exit_status = EXIT_SUCCESS;
    do
    {
        if (-1 == (fd_patch = open(argv[1], O_RDONLY)))
        {
            exit_status = EXIT_FAILURE;
            break;
        }
        if (-1 == (fd1 = open(argv[2], O_RDONLY )))
        {
            exit_status = EXIT_FAILURE;
            break;
        }
        if (-1 == (fd2 = open(argv[3], O_WRONLY | O_TRUNC)))
        {
            errno = 0;
            if (-1 == (fd2 = open(argv[3], O_WRONLY | O_CREAT)))
            {
                exit_status = EXIT_FAILURE;
                break;
            }
        }
        if (-1 == patch_file(fd_patch, fd1, fd2))
        {
            exit_status = EXIT_FAILURE;
            break;
        }
    } while (0);

    if (-1 == close(fd_patch) || -1 == close(fd1) || -1 == close(fd2))
    {
        exit_status = EXIT_FAILURE;
    }

    if (EXIT_FAILURE == exit_status)
    {
        err(exit_status, NULL);
    }

    return 0;
}
