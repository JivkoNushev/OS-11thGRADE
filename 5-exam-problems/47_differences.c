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

struct patch_file_t
{
    uint16_t offset;
    uint8_t original_byte, new_byte;
};

int patch_file(int fd1, int fd2, int fd_patch)
{
    uint8_t fd1_buffer[256] = {0}, fd2_buffer[256] = {0};

    size_t block_size = sizeof fd1_buffer < sizeof fd2_buffer ? sizeof fd1_buffer : sizeof fd2_buffer, block_count = 0;
    int read_fd1_status = 0, read_fd2_status = 0;
    while (0 < (read_fd1_status = read(fd1, fd1_buffer, block_size)) &&
            0 < (read_fd1_status = read(fd2, fd2_buffer, block_size)))
    {
        if(read_fd1_status < read_fd2_status)
        {
            for (size_t i = 0; i < read_fd1_status; i++)
            {
                if(fd1_buffer[i] != fd2_buffer[i])
                {
                    struct patch_file_t temp = {block_count * block_size + i,fd1_buffer[i],fd2_buffer[i]};
                    if(-1 == write(fd_patch, &temp, sizeof(struct patch_file_t)))
                    {
                        return -1;
                    }
                }
            }
            break;
        }
        if(read_fd1_status > read_fd2_status)
        {
            for (size_t i = 0; i < read_fd2_status; i++)
            {
                if(fd1_buffer[i] != fd2_buffer[i])
                {
                    struct patch_file_t temp = {block_count * block_size + i,fd1_buffer[i],fd2_buffer[i]};
                    if(-1 == write(fd_patch, &temp, sizeof(struct patch_file_t)))
                    {
                        return -1;
                    }
                }
            }
            for (size_t i = 0; i < read_fd1_status - read_fd2_status; i++)
            {
                struct patch_file_t temp = {block_count * block_size + i + read_fd2_status,fd1_buffer[i],'\0'};
                if(-1 == write(fd_patch, &temp, sizeof(struct patch_file_t)))
                {
                    return -1;
                }
            }
            break;
        }

        for (size_t i = 0; i < read_fd1_status; i++)
        {
            if(fd1_buffer[i] != fd2_buffer[i])
            {
                struct patch_file_t temp = {block_count * block_size + i,fd1_buffer[i],fd2_buffer[i]};
                if(-1 == write(fd_patch, &temp, sizeof(struct patch_file_t)))
                {
                    return -1;
                }
            }
        }

        if(sizeof fd1_buffer == read_fd1_status && sizeof fd2_buffer == read_fd2_status)
        {
            block_count++;
        }

    }
    if(-1 == read_fd1_status || -1 == read_fd2_status)
    {
        return -1;
    }


    if(-1 == lseek(fd1, 0, SEEK_SET) || -1 == lseek(fd2, 0, SEEK_SET)  || -1 == lseek(fd_patch, 0, SEEK_SET))
    {
        return -1;
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    if(argc != 4)
    {
        errx(EXIT_FAILURE, "Not enough parameters\n\t$ %s [file_name.bin] [file_name.bin] [patch_file_name.bin]\n", argv[0]);
    }
    int fd1 = 0, fd2 = 0, fd_patch = 0;
    
    int exit_status = EXIT_SUCCESS;
    do
    {
        if(-1 == (fd1 = open(argv[1], O_RDONLY)))
        {
            exit_status = EXIT_FAILURE;
            break;
        }
        if(-1 == (fd2 = open(argv[2], O_RDONLY)))
        {
            exit_status = EXIT_FAILURE;
            break;
        }
        if(-1 == (fd_patch = open(argv[3], O_WRONLY)))
        {
            errno = 0;
            if(-1 == (fd_patch = open(argv[3], O_WRONLY | O_CREAT)))
            {
                exit_status = EXIT_FAILURE;
                break;
            }
        }
        if (-1 == patch_file(fd1, fd2, fd_patch))
        {
            exit_status = EXIT_FAILURE;
            break;
        }
    } while (0);

    if(-1 == close(fd1) || -1 == close(fd2) || -1 == close(fd_patch))
    {
        exit_status = EXIT_FAILURE;
    }

    err(exit_status, NULL);
    return 0;
}
