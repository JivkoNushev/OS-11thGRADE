#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "test.h"
#include "../3-utils/utils.h"
#include "wc.h"

int test_wc()
{
    char files[TESTS_COUNT][TESTS_LENGTH] =
    {
        "../0-Tests/file_1",
        "../0-Tests/file_2",
        "../0-Tests/file_3",
        "../0-Tests/file_4",
        "../0-Tests/file_5",
        "../0-Tests/file_6"
    };
    char tests[TESTS_COUNT][TESTS_LENGTH] = 
    {   
        "./main.o", "../0-Tests/file_1  ",
        "./main.o", "../0-Tests/file_2  ", "-l      ",
        "./main.o ../0-Tests/file_3  -c -l      ",
        "./main.o ../0-Tests/file_4  -l -c -c   ",
        "./main.o ../0-Tests/file_5  -p    ",
        "./main.o ../0-Tests/file_6 - l"
    };
    int tests2[TESTS_COUNT] = 
    {   
        2,
        3,
        4,
        5,
        3,
        3
    };

    int fd = -1;
    int tests_count = 0;
    for (int i = 0; i < TESTS_COUNT; i++)
    {
        puts("Testing...\n");
        printf("Test #%d...", i + 1);
        fd = open(files[i], O_RDONLY);
        if (-1 == fd)
        {
            puts("Couldn't open file");
            close(fd);
            exit(2);
        }
        if(1 != wc(fd, tests2[i], tests[tests_count]))
        {
            puts("Failed\n");
            close(fd);
            exit(-1);
        }
        else
        {
            puts("Succeeded\n");
        }
        tests_count += tests2[i];
        close(fd);
    }
    
    return 0;
}

