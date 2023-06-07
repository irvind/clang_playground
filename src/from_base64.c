#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "utils.h"

struct toBaseResult {
    uchar *buffer;
    int result_len;
};

struct toBaseResult convert_from_base64(uchar *input_str, int input_length)
{
    int chunk_count = (input_length / 4);
    assert(chunk_count != 0);
    assert((chunk_count % 4) == 0);
    int buffer_len = chunk_count * 3;

    uchar *conv_input = malloc(input_length);
    for (int i = 0; i < input_length; i++) {
        if (input_str[i] >= 'A' && input_str[i] <= 'Z') {
            conv_input[i] = input_str[i] - 'A';
        } else if (input_str[i] >= 'a' && input_str[i] <= 'z') {
            conv_input[i] = input_str[i] - 'a' + 26;
        } else if (input_str[i] >= '0' && input_str[i] <= '9') {
            conv_input[i] = input_str[i] - '0' + 52;
        } else if (input_str[i] == '+') {
            conv_input[i] = 62;
        } else if (input_str[i] == '/') {
            conv_input[i] = 63;
        } else if (input_str[i] == '=') {
            conv_input[i] = 255;
        }
    }

    uchar *result_buffer = (uchar*)calloc(buffer_len+1, sizeof(uchar));
    int result_len = (chunk_count - 1) * 3;
    for (int i = 0; i < chunk_count; i++) {
        if (conv_input[i*4] == 255 || conv_input[i*4+1] == 255 ||
                    conv_input[i*4+2] == 255 && conv_input[i*4+3] != 255) {
            assert(0);
        }

        if (i < chunk_count - 1) {
            result_buffer[i*3] = (conv_input[i*4] << 2) + conv_input[i*4+1] >> 4;
            result_buffer[i*3+1] = ((conv_input[i*4+1] & 15) << 4) + ((conv_input[i*4+2] & 60) >> 2);
            result_buffer[i*3+2] = conv_input[i*4+3] + (conv_input[i*4+2] & 3) << 6;
        } else {
            if (conv_input[i*4+3] != 255)
                result_len += 3;
            else if (conv_input[i*4+2] != 255)
                result_len += 2;
            else
                result_len += 1;

            result_buffer[i*3+2] = conv_input[i*4+3] == 255 ? 0 :
                conv_input[i*4+3] + (conv_input[i*4+2] & 3) << 6;

            if (conv_input[i*4+2] == 255 && conv_input[i*4+3] == 255)
                result_buffer[i*3+1] = 0;
            else if (conv_input[i*4+2] != 255 && conv_input[i*4+3] == 255)
                result_buffer[i*3+1] = (conv_input[i*4+2] >> 2) + ((conv_input[i*4+1] & 15) << 4);
            else    // conv_input[i*4+2] != 255 && conv_input[i*4+3] != 255
                result_buffer[i*3+1] = ((conv_input[i*4+1] & 15) << 4) + ((conv_input[i*4+2] & 60) >> 2);

            result_buffer[i*3] = (conv_input[i*4] << 2) + conv_input[i*4+1] >> 4;
        }
    }

    free(conv_input);

    struct toBaseResult result;
    result.buffer = result_buffer;
    result.result_len = result_len;

    return result;
}

int main(void)
{
    uchar *stdin_buf = get_buffer_from_stdin();

    struct toBaseResult result = convert_from_base64(stdin_buf, strlen(stdin_buf));
    printf("converted to base64 lorem: %s\n", result.buffer);

    free(stdin_buf);
    free(result.buffer);

    return 0;
}
