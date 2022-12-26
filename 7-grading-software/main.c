#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <err.h>
#include <errno.h>

#define STUDENTS_S 25
#define ANSWERS_S 25


int get_grade(size_t points)
{
    if(points < 13)
    {
        return 2;
    }
    if(points < 16)
    {
        return 3;
    }
    if(points < 19)
    {
        return 4;
    }
    if(points < 22)
    {
        return 5;
    }

    return 6;
}

int path_to_str(const char *directory, size_t i, int flag)
{
    char *dir = (char*)malloc(1); 
    if(NULL == dir)
    {
        return -1;
    }
    if (-1 == sprintf(dir, "%s%lu", directory, i))
    {
        return -1;
    }
    printf("%s", dir);
    int fd = open(dir, flag);
    free(dir);

    return fd;
}

int grade_student(int answer, int result, int correct)
{
    size_t points = 25;

    char answer_buffer[ANSWERS_S * 2 + 1] = {0};
    char correct_buffer[ANSWERS_S * 2 + 1] = {0};

    if(-1 == read(answer, answer_buffer, sizeof answer_buffer))
    {
        return -1;
    }

    if(-1 == read(correct, correct_buffer, sizeof correct_buffer))
    {
        return -1;
    }

    for(size_t i = 0; i < sizeof answer_buffer; i += 2)
    {
        if(answer_buffer[i] != correct_buffer[i])
        {
            points--;
        }
    }

    char grade = '0' + get_grade(points);
    if(-1 == write(result, &grade, 1))
    {
        return -1;
    }

    if(-1 == lseek(correct, 0, SEEK_SET))
    {
        return -1;
    }
    return 0;
}

int main(int argc, char const *argv[])
{

    int correct = open(argv[2], O_RDONLY);
    if(-1 == correct)
    {
        err(EXIT_FAILURE, NULL);
    }

    for(size_t i = 1; i <= STUDENTS_S; i++)
    {
        int answer = path_to_str(argv[1], i, O_RDONLY);
        if(-1 == answer)
        {
            close(answer);
            close(correct);
            err(EXIT_FAILURE, NULL);
        }
        int result = path_to_str(argv[3], i, O_WRONLY | O_TRUNC);
        if(-1 == result)
        {
            close(answer);
            close(correct);
            err(EXIT_FAILURE, NULL);
        }

        if(-1 == grade_student(answer, result, correct))
        {
            close(answer);
            close(result);
            close(correct);
            err(EXIT_FAILURE, NULL);
        }

        close(answer);
        close(result);
    }

    close(correct);

    return 0;
}
