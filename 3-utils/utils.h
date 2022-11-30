#ifndef UTILS
#define UTILS

#define LINE_BACKWARDS 1
#define LINE_FORWARD 0

int strlen_(const char *str);

int strcmp_(const char *left, const char *right);

char *strcat_(char *one, char *two);

void swap_int_(int *a, int *b);

void swap_(void *a, void *b, int size);

char *strdub_(const char *string);

char *read_line_(int fd, int mode, ...); // make it get_line

int count_bytes(int fd);

int count_lines(int fd);

unsigned int abs_(int n);

int print_file(int fd);

int read_line(int fd); // returns offset

int cpyfile(int to, int from);

#endif