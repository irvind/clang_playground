#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

uchar *get_buffer_from_stdin()
{
    int fib_prev = 21;
    int fib_cur = 34;
    int buf_size = fib_cur;
    int idx = 0;

    uchar *buf = malloc(buf_size+1);

    while (1) {
        int read_count = fread(
            buf + idx,
            sizeof(unsigned char),
            buf_size - idx,
            stdin
        );
        idx += read_count;

        if (feof(stdin))
            break;
        if (ferror(stdin)) {
            printf("There was an error during stdin read\n");
            break;
        }

        int fib_tmp = fib_cur;
        fib_cur = fib_cur + fib_prev;
        fib_prev = fib_tmp;
        buf_size = fib_cur;

        buf = realloc(buf, buf_size+1);
    }

    return buf;
}
