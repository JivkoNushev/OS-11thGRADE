#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "test.h"
#include "utils.h"

int test_strlen_()
{
    char tests[TESTS_COUNT][TESTS_LENGTH] = {"test2", "test 3 ", " i i ii", " opop da ][]  = ", "lol? ", "    pooopy  pap"};
    char results[TESTS_COUNT] = {5, 7, 7, 16, 5, 15};
    int test_status = 0;
    for (int i = 0; i < TESTS_COUNT; i++)
    {
        puts("Testing...\n");
        printf("Test #%d...", i + 1);
        if(results[i] != strlen_(tests[i]))
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

int test_strcmp_()
{
    char tests[TESTS_COUNT*2][TESTS_LENGTH] = 
    {   "test2", "test2", 
        " i i ii", " opop da ][]  = ", 
        "lol? ", "lol?",
        "yoan e ", "mnogo gotin",
        " znae", "yoan mnogo",
        "yoan e mnogo po-","yoan e mnogo po-"
    };
    int results[TESTS_COUNT] = 
    {   0, 
        -1, 
        1, 
        1, 
        -1, 
        0
    };
    int test_status = 0;
    for (int i = 0, j = 0; i < TESTS_COUNT; i++, j+=2)
    {
        puts("Testing...\n");
        printf("Test #%d...", i + 1);
        if(strcmp_(tests[j], tests[j+1]) != results[i])
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

int test_strcat_()
{
    char tests[TESTS_COUNT*2][TESTS_LENGTH] = 
    {   "test2", "test 3 ", 
        " i i ii", " opop da ][]  = ", 
        "lol? ", "    pooopy  pap",
        "yoan e ", "mnogo gotin",
        "yoan mnogo", " znae",
        "yoan e mnogo po-","dobur"
    };
    char results[TESTS_COUNT][TESTS_LENGTH] = 
    {   "test2test 3 ", 
        " i i ii opop da ][]  = ", 
        "lol?     pooopy  pap",
        "yoan e mnogo gotin",
        "yoan mnogo znae",
        "yoan e mnogo po-dobur"
    };
    int test_status = 0;
    for (int i = 0, j = 0; i < TESTS_COUNT; i++, j+=2)
    {
        puts("Testing...\n");
        printf("Test #%d...", i + 1);
        if(0 != strcmp_(strcat_(tests[j], tests[j+1]), results[i]))
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

int test_swap_int_()
{
    int tests[TESTS_COUNT * 2] = 
    {   
        1, 2,
        3, 4,
        5, 6,
        7, 8,
        9, 10,
        11, 12
    };
    int results[TESTS_COUNT * 2] = 
    {   
        2, 1,
        4, 3,
        6, 5,
        8, 7,
        10, 9,
        12, 11
    };
    int test_status = 0;
    for (int i = 0, j = 0; i < TESTS_COUNT; i++, j+=2)
    {
        swap_int_(&tests[j], &tests[j + 1]);
    }
    
    for (int i = 0, j = 0; i < TESTS_COUNT; i++, j+=2)
    {
        puts("Testing...\n");
        printf("Test #%d...", i + 1);
        if(results[j] != tests[j] || results[j+1] != tests[j+1])
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

int test_swap_()
{
    int int_1 = 1, int_2 = 2, int_res_1 = 2, int_res_2 = 1;
    swap_(&int_1, &int_2, sizeof(int));

    long long int long_int_1 = 1, long_int_2 = 2, long_int_res_1 = 2, long_int_res_2 = 1;
    swap_(&long_int_1, &long_int_2, sizeof(long long int));

    char char_1 = '1', char_2 = '2', char_res_1 = '2', char_res_2 = '1';
    swap_(&char_1, &char_2, sizeof(char));

    // double double_1 = 0.1, double_2 = 0.2, double_res_1 = 0.2, double_res_2 = 0.1;
    // swap((void*)double_1, (void*)double_2, sizeof(double));

    int test_status = 0;
    int i = 0;
    puts("Testing...\n");
    printf("Test #%d...", i + 1);
    if(int_1 != int_res_1 || int_2 != int_res_2)
    {
        puts("Failed\n");
        test_status = -1;
    }
    else
    {
        puts("Succeeded\n");
    }
    i++;

    puts("Testing...\n");
    printf("Test #%d...", i + 1);
    if(long_int_1 != long_int_res_1 || long_int_2 != long_int_res_2)
    {
        puts("Failed\n");
        test_status = -1;
    }
    else
    {
        puts("Succeeded\n");
    }
    i++;

    puts("Testing...\n");
    printf("Test #%d...", i + 1);
    if(char_1 != char_res_1 || char_2 != char_res_2)
    {
        puts("Failed\n");
        test_status = -1;
    }
    else
    {
        puts("Succeeded\n");
    }
    i++;

    // puts("Testing...\n");
    // printf("Test #%d...", i + 1);
    // if(double_1 != double_res_1 || double_2 != double_res_2)
    // {
    //     puts("Failed\n");
    //     test_status = -1;
    // }
    // else
    // {
    //     puts("Succeeded\n");
    // }
    // i++;

    return test_status;
}

int test_strdub_()
{
    char tests[TESTS_COUNT][TESTS_LENGTH] = 
    {   "test2",
        "test 3 ",
        " i i ii",
        " opop da ][]  = ",
        "lol? ",
        "    pooopy  pap"
    };

    int test_status = 0;
    char * buffer = 0;
    for (int i = 0; i < TESTS_COUNT; i++)
    {
        puts("Testing...\n");
        printf("Test #%d...", i + 1);
        if(0 != strcmp_(buffer = strdub_(tests[i]), tests[i]))
        {
            puts("Failed\n");
            test_status = -1;
        }
        else
        {
            puts("Succeeded\n");
        }
        free(buffer);
    }
    
    return test_status;
}

int test_read_line_()
{
    char tests[TESTS_COUNT][TESTS_LENGTH] = 
    {   
        "./Tests/File_tests/file_1",
        "./Tests/File_tests/file_2",
        "./Tests/File_tests/file_3",
        "./Tests/File_tests/file_4",
        "./Tests/File_tests/file_5",
        "./Tests/File_tests/file_6"
    };
    char results[TESTS_COUNT][TESTS_LENGTH] = 
    {   
        "text",
        "dasmdlmas dsadsd sad sad as5d9 as6d65as",
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        "cjokjndkf; d,sv,mds",
        " ",
        "                    lkdsadsadsa"
    };

    int test_status = 0;
    int fd = -1;
    char * buffer = 0;
    for (int i = 0; i < TESTS_COUNT; i++)
    {
        puts("Testing...\n");
        printf("Test #%d...", i + 1);
        fd = open(tests[i], O_RDONLY);
        if (-1 == fd)
        {
            puts("Couldn't open file");
            exit(2);
        }
        if(0 != strcmp_(buffer = read_line_(fd), results[i]))
        {
            puts("Failed\n");
            test_status = -1;
        }
        else
        {
            puts("Succeeded\n");
        }
        free(buffer);
    }
    
    return test_status;
}

int test_count_bytes()
{
    char tests[TESTS_COUNT][TESTS_LENGTH] = 
    {   
        "./Tests/File_tests/file_1",
        "./Tests/File_tests/file_2",
        "./Tests/File_tests/file_3",
        "./Tests/File_tests/file_4",
        "./Tests/File_tests/file_5",
        "./Tests/File_tests/file_6"
    };
    int results[TESTS_COUNT] = 
    {   
        5,
        49,
        161,
        37,
        14,
        52
    };

    int test_status = 0;
    int fd = -1;
    for (int i = 0; i < TESTS_COUNT; i++)
    {
        puts("Testing...\n");
        printf("Test #%d...", i + 1);
        fd = open(tests[i], O_RDONLY);
        if (-1 == fd)
        {
            puts("Couldn't open file");
            exit(2);
        }
        if(results[i] != count_bytes(fd))
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

int test_count_lines()
{
    char tests[TESTS_COUNT][TESTS_LENGTH] = 
    {   
        "./Tests/File_tests/file_1",
        "./Tests/File_tests/file_2",
        "./Tests/File_tests/file_3",
        "./Tests/File_tests/file_4",
        "./Tests/File_tests/file_5",
        "./Tests/File_tests/file_6"
    };
    int results[TESTS_COUNT] = 
    {   
        1,
        3,
        1,
        5,
        2,
        1
    };

    int test_status = 0;
    int fd = -1;
    for (int i = 0; i < TESTS_COUNT; i++)
    {
        puts("Testing...\n");
        printf("Test #%d...", i + 1);
        fd = open(tests[i], O_RDONLY);
        if (-1 == fd)
        {
            puts("Couldn't open file");
            exit(2);
        }
        if(results[i] != count_lines(fd))
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