// Helpful links:
// https://en.wikipedia.org/wiki/Base64
// https://theasciicode.com.ar/
// https://www.rapidtables.com/convert/number/decimal-to-binary.html

// Usage: cat lorem_content.txt | ./to_base64

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char uchar;
typedef unsigned short ushort;

// TODO: check allocation errors

uchar* convert_to_base64(uchar *input_str, int input_length)
{
    int chunk_count = (input_length / 3);
    if (input_length % 3 != 0)
        chunk_count += 1;
    int buffer_len = chunk_count * 4;

    uchar *result_buffer = (uchar*)calloc(buffer_len+1, sizeof(uchar));
    for (int chunk_idx = 0, input_idx = 0; chunk_idx < chunk_count; chunk_idx++, input_idx++) {
        if ((chunk_idx < chunk_count - 1) || (input_length % 3 == 0)) {
            ushort sh1 = (input_str[input_idx*3] << 8) + input_str[input_idx*3+1];
            ushort sh2 = (input_str[input_idx*3+1] << 8) + input_str[input_idx*3+2];

            result_buffer[chunk_idx*4] = input_str[input_idx*3] >> 2;
            result_buffer[chunk_idx*4+1] = (ushort)(sh1 << 6) >> 10;
            result_buffer[chunk_idx*4+2] = (ushort)(sh2 << 4) >> 10;
            result_buffer[chunk_idx*4+3] = (uchar)(input_str[(input_idx*3)+2] << 2) >> 2;
        } else if (input_length % 3 == 2) {
            ushort sh1 = (input_str[input_idx*3] << 8) + input_str[input_idx*3+1];

            result_buffer[chunk_idx*4] = input_str[input_idx*3] >> 2;
            result_buffer[chunk_idx*4+1] = (ushort)(sh1 << 6) >> 10;
            result_buffer[chunk_idx*4+2] = (uchar)(input_str[input_idx*3+1] << 4) >> 2;
            result_buffer[chunk_idx*4+3] = 255;
        } else {    // input_length % 3 == 1
            result_buffer[chunk_idx*4] = input_str[input_idx*3] >> 2;
            result_buffer[chunk_idx*4+1] = (uchar)(input_str[input_idx*3] << 6) >> 2;
            result_buffer[chunk_idx*4+2] = 255;
            result_buffer[chunk_idx*4+3] = 255;
        }

        for (int i = chunk_idx*4; i < chunk_idx*4+4; i++) {
            uchar sextet  = result_buffer[i];
            if (sextet <= 25) {
                // Uppercase characters 65
                result_buffer[i] = 'A' + sextet;
            } else if (sextet <= 51) {
                // Lowercase characters
                result_buffer[i] = 'a' + (sextet - 26);
            } else if (sextet <= 61) {
                // Numbers
                result_buffer[i] = '0' + (sextet - 52);
            } else if (sextet == 62) {
                result_buffer[i] = '+';
            } else if (sextet == 63) {
                result_buffer[i] = '/';
            } else {    // sextet == 255
                result_buffer[i] = '=';
            }
        }
    }

    result_buffer[buffer_len] = '\0';

    return result_buffer;
}

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

int main(void)
{
    uchar *lorem = 
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
        "Vivamus luctus fringilla volutpat. Etiam suscipit, neque "
        "et scelerisque rutrum, tortor libero efficitur dui, sed "
        "placerat nulla sem vitae dui. Mauris ac eros blandit, "
        "feugiat leo in, ornare mi. Integer mollis risus quis dui "
        "dictum volutpat. Vivamus dictum justo eu placerat dictum. "
        "Class aptent taciti sociosqu ad litora torquent per conubia "
        "nostra, per inceptos himenaeos. Nam sodales volutpat ex. "
        "Pellentesque nec sollicitudin massa. Fusce maximus sapien "
        "tristique tempus gravida. Morbi eu porttitor sem. Maecenas "
        "sit amet mi at dui accumsan molestie";
    uchar *stdin_buf = get_buffer_from_stdin();
    // printf("%s\n", stdin_buf);
 
    // uchar *result_base64 = convert_to_base64(lorem, strlen(lorem));
    uchar *result_base64 = convert_to_base64(stdin_buf, strlen(stdin_buf));
    printf("converted to base64 lorem: %s\n", result_base64);

    free(result_base64);
    free(stdin_buf);

    return 0;
}
