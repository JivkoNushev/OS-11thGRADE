#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "../3-utils/utils.h"
#include "../3-utils/utils.c"

#include "wc.h"
#include "test.h"

int main(int argc, const char *argv[])
{
    if(0 == test_wc())
    {
        puts("TEST SUCCEEDED\n");
    }
    else
    {
        puts("TEST FAILED\n");
        exit(1);
    }

    return 0;
}
