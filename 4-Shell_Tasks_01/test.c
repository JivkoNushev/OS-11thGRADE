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
    char tests[TESTS_COUNT][TESTS_LENGTH] = 
    {   
        "./main.o ../0-Tests/file_1      ",
        "./main.o ../0-Tests/file_2  -l      ",
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

    int test_status = 0;
    int fd = -1;
    for (int i = 0; i < TESTS_COUNT; i++)
    {
        puts("Testing...\n");
        printf("Test #%d...", i + 1);
        fd = open(tests[1], O_RDONLY);
        if (-1 == fd)
        {
            puts("Couldn't open file");
            exit(2);
        }
        if(1 != wc(fd, tests2[i], (const char**)tests))
        {
            puts("Failed\n");
            test_status = -1;
        }
        else
        {
            puts("Succeeded\n");
        }
    }
    
    return test_status;
}