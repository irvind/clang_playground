#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 16

int main(void) 
{
    unsigned char buf[BUF_SIZE+1];

    printf("eof reached: %d\n", feof(stdin));

    int read_count = fread(buf, sizeof(unsigned char), BUF_SIZE, stdin);
    printf("first read_count: %d\n", read_count);
    printf("eof reached: %d\n", feof(stdin));

    read_count = fread(buf, sizeof(unsigned char), BUF_SIZE, stdin);
    printf("second read_count: %d\n", read_count);
    return 0;
}
