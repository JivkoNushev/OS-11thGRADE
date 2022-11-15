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

struct idx_file
{
    uint16_t offset;
    uint8_t length, reserved;
};

int is_capital(uint8_t letter)
{
    return 'A' <= letter && letter <= 'Z';
}

int dat_idx(int fd_dat1, int fd_idx1, int fd_dat2, int fd_idx2)
{
    struct idx_file idx_block = {0,0,0}; 
    uint8_t block[256] = {0};

    size_t is_cap = 0, len = 0;
    int read_dat_status = 0, read_idx_status = 0;
    while(sizeof(struct idx_file) == (read_idx_status = read(fd_idx1, &idx_block, sizeof(struct idx_file))))
    {
        struct idx_file new_idx_block = {0,0,0};

        if(-1 == lseek(fd_dat1, idx_block.offset, SEEK_SET))
        {
            return -1;
        }
        while(0 < (read_dat_status =  read(fd_dat1, block, sizeof block)))
        {
            if(0 == is_cap)
            {
                if(0 == (is_cap = is_capital(block[0])))
                { 
                    break;
                }
                if(-1 == (new_idx_block.offset = lseek(fd_dat2, 0, SEEK_CUR)))
                {
                    return -1;
                }
            }

            if(read_dat_status > new_idx_block.length)
            {
                if(-1 == write(fd_dat2, block, new_idx_block.length))
                {
                    return -1;
                }
                len += new_idx_block.length;
                break;
            }
            new_idx_block.length -= read_dat_status;
            len += read_dat_status;

            if(-1 == write(fd_dat2, block, read_dat_status))
            {
                return -1;
            }
        }
        if(-1 == read_dat_status)
        {
            return -1;
        }

        if(1 == is_cap)
        {
            new_idx_block.length = len;
            if(-1 == write(fd_idx2, &new_idx_block, sizeof(struct idx_file)))
            {
                return -1;
            }
        }

        is_cap = 0;
        len = 0;
    }
    if(-1 == read_idx_status)
    {
        return -1;
    }
    if(0 != read_idx_status)
    {
        if(-1 == close(fd_dat1) || -1 == close(fd_idx1) || -1 == close(fd_dat2) || -1 == close(fd_idx2))
        {
            return -1;
        }
        errx(EXIT_FAILURE, "Non valid structure for idx_file\n");
    }

    if(-1 == lseek(fd_dat1, 0, SEEK_SET) || -1 == lseek(fd_idx1, 0, SEEK_SET)  || -1 == lseek(fd_dat2, 0, SEEK_SET) || -1 == lseek(fd_idx2, 0, SEEK_SET))
    {
        return -1;
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    if(argc != 5)
    {
        errx(EXIT_FAILURE, "Not enough parameters\n\t$ %s [file_name.dat] [file_name.idx] [file_name.dat] [file_name.idx]\n", argv[0]);
    }
    int fd_dat1 = 0, fd_dat2 = 0, fd_idx1 = 0, fd_idx2 = 0;
    
    int exit_status = EXIT_SUCCESS;
    do
    {
        if(-1 == (fd_dat1 = open(argv[1], O_RDONLY)))
        {
            exit_status = EXIT_FAILURE;
            break;
        }
        if(-1 == (fd_idx1 = open(argv[2], O_RDONLY)))
        {
            exit_status = EXIT_FAILURE;
            break;
        }
        if(-1 == (fd_dat2 = open(argv[3], O_WRONLY)))
        {
            errno = 0;
            if(-1 == (fd_dat2 = open(argv[3], O_WRONLY | O_CREAT)))
            {
                exit_status = EXIT_FAILURE;
                break;
            }
        }
        if(-1 == (fd_idx2 = open(argv[4], O_WRONLY)))
        {
            errno = 0;
            if(-1 == (fd_idx2 = open(argv[4], O_WRONLY | O_CREAT)))
            {
                exit_status = EXIT_FAILURE;
                break;
            }
        }
        if (-1 == dat_idx(fd_dat1, fd_idx1, fd_dat2, fd_idx2))
        {
            exit_status = EXIT_FAILURE;
            break;
        }
    } while (0);

        
    if(-1 == close(fd_dat1) || -1 == close(fd_idx1) || -1 == close(fd_dat2) || -1 == close(fd_idx2))
    {
        exit_status = EXIT_FAILURE;
    }

    err(exit_status, NULL);
    return 0;
}
