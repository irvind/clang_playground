// Helpful links:
// https://en.wikipedia.org/wiki/Base64
// https://theasciicode.com.ar/
// https://www.rapidtables.com/convert/number/decimal-to-binary.html

// Build: gcc -o to_base64 to_base64.c utils.c
// Usage: cat lorem_content.txt | ./to_base64

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

#define MASK_11 3
#define MASK_1111 15
#define MASK_11111100 124
#define MASK_00111111 63
#define MASK_1111110000 1008
#define MASK_111111000000 4032
#define EQUALS_SIGN_CODE 255


uchar* convert_to_base64(uchar *input_str, int input_length)
{
    int chunk_count = (input_length / 3);
    if (input_length % 3 != 0)
        chunk_count += 1;
    int buffer_len = chunk_count * 4;

    uchar *result_buffer = (uchar*)malloc(buffer_len+1);
    if (result_buffer == NULL)
        return NULL;

    for (int i = 0; i < chunk_count; i++) {
        if ((i < chunk_count - 1) || (input_length % 3 == 0)) {
            ushort sh1 = (input_str[i*3] << 8) + input_str[i*3+1];
            ushort sh2 = (input_str[i*3+1] << 8) + input_str[i*3+2];

            result_buffer[i*4] = (input_str[i*3] & MASK_11111100) >> 2;
            result_buffer[i*4+1] = (sh1 & MASK_1111110000) >> 4;
            result_buffer[i*4+2] = (sh2 & MASK_111111000000) >> 6;
            result_buffer[i*4+3] = input_str[(i*3)+2] & MASK_00111111;
        } else if (input_length % 3 == 2) {
            ushort sh1 = (input_str[i*3] << 8) + input_str[i*3+1];

            result_buffer[i*4] = (input_str[i*3] & MASK_11111100) >> 2;
            result_buffer[i*4+1] = (sh1 & MASK_1111110000) >> 4;
            result_buffer[i*4+2] = (input_str[i*3+1] & MASK_1111) << 2;
            result_buffer[i*4+3] = EQUALS_SIGN_CODE;
        } else {    // input_length % 3 == 1
            result_buffer[i*4] = input_str[i*3] >> 2;
            result_buffer[i*4+1] = (input_str[i*3] & MASK_11) << 4;
            result_buffer[i*4+2] = EQUALS_SIGN_CODE;
            result_buffer[i*4+3] = EQUALS_SIGN_CODE;
        }

        for (int j = i*4; j < i*4+4; j++) {
            uchar sextet  = result_buffer[j];
            if (sextet <= 25) {
                // Uppercase characters 65
                result_buffer[j] = 'A' + sextet;
            } else if (sextet <= 51) {
                // Lowercase characters
                result_buffer[j] = 'a' + (sextet - 26);
            } else if (sextet <= 61) {
                // Numbers
                result_buffer[j] = '0' + (sextet - 52);
            } else if (sextet == 62) {
                result_buffer[j] = '+';
            } else if (sextet == 63) {
                result_buffer[j] = '/';
            } else {    // sextet == 255
                result_buffer[j] = '=';
            }
        }
    }

    result_buffer[buffer_len] = '\0';

    return result_buffer;
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
    if (stdin_buf == NULL) {
        printf("Cannot allocate memory\n");
        exit(EXIT_FAILURE);
    }
 
    // uchar *result_base64 = convert_to_base64(lorem, strlen(lorem));
    uchar *result_base64 = convert_to_base64(stdin_buf, strlen(stdin_buf));
    if (result_base64 == NULL) {
        printf("Cannot allocate memory\n");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", result_base64);

    free(result_base64);
    free(stdin_buf);

    return 0;
}
