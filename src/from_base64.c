#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "utils.h"

#define CONV_B64_ERR_MEM_ALLOCATION 0
#define CONV_B64_ERR_INVALID_LENGTH 1
#define CONV_B64_ERR_INVALID_BASE64 2

#define MASK_11 3
#define MASK_1111 15
#define MASK_111100 60

#define EQUALS_SIGN_CODE 255

uchar* convert_from_base64(uchar *input_str, int input_length, int *output_length, int *error)
{
    if ((input_length == 0) || (input_length % 4) != 0) {
        *error = CONV_B64_ERR_INVALID_LENGTH;
        return NULL;
    }

    uchar *conv_input = malloc(input_length);
    if (conv_input == NULL) {
        *error = CONV_B64_ERR_MEM_ALLOCATION;
        return NULL;
    }

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
            conv_input[i] = EQUALS_SIGN_CODE;
        }
    }

    int chunk_count = input_length / 4;
    int buffer_len = chunk_count * 3;
    int result_len = (chunk_count - 1) * 3;
    uchar *result_buffer = (uchar*)malloc(buffer_len+1);

    for (int i = 0; i < chunk_count; i++) {
        if (conv_input[i*4] == EQUALS_SIGN_CODE || conv_input[i*4+1] == EQUALS_SIGN_CODE ||
                conv_input[i*4+2] == EQUALS_SIGN_CODE && conv_input[i*4+3] != EQUALS_SIGN_CODE
        ) {
            free(conv_input);
            *error = CONV_B64_ERR_INVALID_BASE64;
            return NULL;
        }

        if (i < chunk_count - 1) {
            result_buffer[i*3] = (conv_input[i*4] << 2) + (conv_input[i*4+1] >> 4);
            result_buffer[i*3+1] = ((conv_input[i*4+1] & MASK_1111) << 4) + ((conv_input[i*4+2] & MASK_111100) >> 2);
            result_buffer[i*3+2] = conv_input[i*4+3] + ((conv_input[i*4+2] & MASK_11) << 6);
        } else {
            if (conv_input[i*4+3] != EQUALS_SIGN_CODE)
                result_len += 3;
            else if (conv_input[i*4+2] != EQUALS_SIGN_CODE)
                result_len += 2;
            else
                result_len += 1;

            result_buffer[i*3+2] = conv_input[i*4+3] == EQUALS_SIGN_CODE ? 0 :
                conv_input[i*4+3] + ((conv_input[i*4+2] & MASK_11) << 6);

            if (conv_input[i*4+2] == EQUALS_SIGN_CODE && conv_input[i*4+3] == EQUALS_SIGN_CODE)
                result_buffer[i*3+1] = 0;
            else if (conv_input[i*4+2] != 255 && conv_input[i*4+3] == EQUALS_SIGN_CODE)
                result_buffer[i*3+1] = (conv_input[i*4+2] >> 2) + ((conv_input[i*4+1] & MASK_1111) << 4);
            else    // conv_input[i*4+2] != 255 && conv_input[i*4+3] != 255
                result_buffer[i*3+1] = ((conv_input[i*4+1] & MASK_1111) << 4) + ((conv_input[i*4+2] & MASK_111100) >> 2);

            result_buffer[i*3] = (conv_input[i*4] << 2) + (conv_input[i*4+1] >> 4);
        }
    }

    free(conv_input);

    *output_length = result_len;
    return result_buffer;
}

int main(void)
{
    uchar *stdin_buf = get_buffer_from_stdin();
    if (stdin_buf == NULL) {
        printf("Cannot allocate memory\n");
        exit(EXIT_FAILURE);
    }

    int result_length, error;
    uchar *result = convert_from_base64(stdin_buf, strlen(stdin_buf), &result_length, &error);
    if (result == NULL) {
        free(stdin_buf);
        if (error == CONV_B64_ERR_MEM_ALLOCATION) 
            printf("Cannot allocate memory\n");
        else if (error == CONV_B64_ERR_INVALID_LENGTH)
            printf("Invalid base64 length\n");
        else if (error == CONV_B64_ERR_INVALID_BASE64)
            printf("Base64 contains errors\n");
        else
            printf("Unknown error\n");
        exit(EXIT_FAILURE);
    }

    fwrite(result, sizeof(uchar), result_length, stdout);
    putchar('\n');

    free(stdin_buf);
    free(result);

    return 0;
}
