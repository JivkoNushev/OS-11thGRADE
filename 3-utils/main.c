#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>


#include <fcntl.h>

#include "utils.h"
#include "test.h"

int main(int argc, char const *argv[])
{
    if(0 == test_read_line_())
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