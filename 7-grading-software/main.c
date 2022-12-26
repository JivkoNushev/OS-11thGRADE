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

int strlen_(const char *str)
{
    unsigned int len = 0;
    if (NULL == str)
    {
        return 0;
    }
    for (const char *l = str; '\0' != *l; l++, len++)
        ;
    return len;
}

char* int_to_str(int n)
{
    char number[11] = {0};
    
    int i = 9;
    while (0 != n)
    {
        number[i--] = n % 10;
        n /= 10;
    }
    int len = strlen_(number);


    char *result = (char*)malloc(len + 1);
    if(NULL == result)
    {
        return NULL;
    }

    for(int i = 10 - len, j = 0; i < 10; j++, i++)
    {
        result[j] = number[i];
    }
    printf("number_str: %s\n", result);

    result[len] = '\0';


    return result;    
}

char* cat_size_t(char *str, size_t number)
{   
    char *number_str = int_to_str((int)number);
    if(NULL == number_str)
    {
        return NULL;
    }

    int str_s = strlen_(str);
    int number_str_s = strlen_(str);

    char *result = (char*)malloc(str_s + number_str_s + 1);
    if(NULL == result)
    {
        return NULL;
    }
    printf("str: %s\n", str);

    for(int i = 0; i < str_s; i++)
    {
        result[i] = str[i]; 
    }
    for(int i = 0; i < number_str_s; i++)
    {
        result[i + str_s] = number_str[i]; 
    }
    result[str_s + number_str_s] = '\0';


    free(str);
    free(number_str);
    return result;
}

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
    char *dir = cat_size_t(dir, i);
    if (NULL == dir)
    {
        return -1;
    }
    printf("%s\n", dir);

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
