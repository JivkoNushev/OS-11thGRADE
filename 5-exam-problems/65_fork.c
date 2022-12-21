#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <err.h>
#include <errno.h>

#include <time.h>

//int Q(int input)
//{
//    if (input > 1000000)
//    {
//        return -1;
//    }
//
//    for(int i = 0; i < input; i++)
//    {
//        printf("%d ", i);
//    }
//    printf("\n");
//
//    return 0;
//}
//
//int P(int time_, int (*q)(), int input)
//{
//    int fd = open("run.log", O_WRONLY | O_APPEND);
//    if(-1 == fd)
//    {
//        return -1;
//    }
//    
//    do
//    {
//        int pid1 = fork();
//        time_t t_start = time(NULL);
//        int exit_status = q(input*=2);
//        time_t t_end = time(NULL);
//
//        if (-1 == dprintf(fd, "%d %d %d", t_start, t_end, exit_status))
//        {
//            return -1;
//        }
//
//        if(exit_status != 0 && t_end - t_start < time_)
//        {
//            break;
//        }
//
//    }while(1);
//
//    return 0;
//}
//

int main(int argc, char const *argv[])
{
    pid_t f = fork();

    if(0 == f)
    {
        printf("%d", getpid());
        printf("Child");
    }
    else //if(getpid() == f)
    {
        printf("%d", getpid());
        printf("Parent");
    }

    return 0;
}
