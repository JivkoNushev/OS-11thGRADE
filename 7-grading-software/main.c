#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <err.h>
#include <errno.h>

#define DEBUG_MODE 0

#define DEBUG if(DEBUG_MODE){
#define DEBUG_END } 

#define STUDENTS_S 25
#define ANSWERS_S 25

// function declarations
int open_file(const char *directory, int iterator, int flag);
int grade_student(int answer, int result, int correct);
int get_grade(int points);
char* cat_int(const char *str, int number);
char* int_to_str(int n);
int strlen_(const char *str);


// returns the length of a string
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

// returns the string of a number
char* int_to_str(int n)
{
    char number[11] = {0};
    
    // split the number into digits
    int number_s = 0;
    for(number_s = 0; 0 != n; number_s++)
    {
        number[number_s] = '0' + n % 10;
        n /= 10;
    }

    // allocate memory for the string
    char *result = (char*)malloc(number_s + 1);
    if(NULL == result)
    {
        return NULL;
    }

    // put the digits in the correct order
    for(int i = 0; i < number_s; i++)
    {
        result[i] = number[(number_s - 1) - i];
    }
    result[number_s] = '\0';

    return result;    
}

char* cat_int(const char *str, int number)
{   
    // get the string of a number
    char *number_str = int_to_str(number);
    if(NULL == number_str)
    {
        return NULL;
    }

    // get the length of the strings
    int str_s = strlen_(str);
    int number_str_s = strlen_(number_str);

    // allocate memory for the concatenated string
    char *result = (char*)malloc(str_s + number_str_s + 1);
    if(NULL == result)
    {
        return NULL;
    }

    // concatenate both strings
    for(int i = 0; i < str_s; i++)
    {
        result[i] = str[i]; 
    }
    for(int i = 0; i < number_str_s; i++)
    {
        result[i + str_s] = number_str[i]; 
    }
    result[str_s + number_str_s] = '\0';

    free(number_str);
    return result;
}

int get_grade(int points)
{
    // can't figure a simpler formula
    return 2 + (points - 10 > 0 ? (points == 25 ? points - 11 : points - 10) : 0) / 3;
}

int grade_student(int answer, int result, int correct)
{
    size_t points = 25;

    char answer_buffer[ANSWERS_S * 2] = {0};
    char correct_buffer[ANSWERS_S * 2] = {0};

    // read the student answers
    if(sizeof answer_buffer != read(answer, answer_buffer, sizeof answer_buffer))
    {
        return -1;
    }

    // read the correct answers
    if(sizeof correct_buffer != read(correct, correct_buffer, sizeof correct_buffer))
    {
        return -1;
    }

    // find the number of points the student has
    for(size_t i = 0; i < sizeof answer_buffer; i += 2)
    {
        // faster for smarter students
        if(answer_buffer[i] != correct_buffer[i])
        {
            points--;                                   
        }
    }
    
    // convert the points into a grade and write it to the student grade file
    char grade = '0' + get_grade((int)points);
    if(-1 == write(result, &grade, 1))
    {
        return -1;
    }

    // lseek the correct answers file descriptor to the start of the file
    if(-1 == lseek(correct, 0, SEEK_SET))
    {
        return -1;
    }

    return 0;
}


int open_file(const char *directory, int iterator, int flag)
{
    // get the path to the file    
    char *path = cat_int(directory, iterator);
    if (NULL == path)
    {
        return -1;
    }

DEBUG
    // debug the paths
    printf("%s\n", path); 
DEBUG_END

    // open the file
    int fd = open(path, flag);

    free(path);
    return fd;
}

int main(int argc, char const *argv[])
{
    // open answers sheet
    int correct = open(argv[2], O_RDONLY);
    if(-1 == correct)
    {
        err(EXIT_FAILURE, NULL);
    }

    for(int i = 1; i <= STUDENTS_S; i++)
    {
        // open student answers
        int answer = open_file(argv[1], i, O_RDONLY);
        if(-1 == answer)
        {
            close(answer);
            close(correct);
            err(EXIT_FAILURE, NULL);
        }

        // open student grade file
        int result = open_file(argv[3], i, O_WRONLY | O_TRUNC);
        if(-1 == result)
        {
            close(answer);
            close(correct);
            err(EXIT_FAILURE, NULL);
        }

        // grade student
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
