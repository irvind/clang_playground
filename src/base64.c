// Hello
// https://rextester.com/l/c_online_compiler_gcc

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char uchar;
typedef unsigned short ushort;

uchar* convert_to_base64(uchar *input_str, int input_length)
{
    printf("In convert_to_base64\n");
    printf("input_length = %d\n", input_length);
    
    int chunk_count = (input_length / 3);
    if (input_length % 3 != 0)
        chunk_count += 1;
    
    int buffer_len = chunk_count * 4;
    
    printf("buffer_len = %d\n", buffer_len);
    
    uchar *result_buffer = (uchar*)calloc(buffer_len+1, sizeof(uchar));
    int input_idx = 0;
    for (int chunk_idx = 0; chunk_idx < chunk_count; chunk_idx++) {
        if ((chunk_idx < chunk_count - 1) || (input_length % 3 == 0)) {
            ushort sh1 = (input_str[input_idx*3] << 8) + input_str[input_idx*3+1];
            ushort sh2 = (input_str[input_idx*3+1] << 8) + input_str[input_idx*3+2];

            result_buffer[chunk_idx*4] = (uchar)(input_str[input_idx*3] >> 2);
            result_buffer[chunk_idx*4+1] = (uchar)( (uchar)(sh1 << 6) >> 10);
            result_buffer[chunk_idx*4+2] = (uchar)( (uchar)(sh2 << 4) >> 10);
            result_buffer[chunk_idx*4+3] = (uchar)( (uchar)(input_str[(input_idx*3)+2] << 2) >> 2);
        } else if (input_length % 3 == 2) {
            ushort sh1 = (input_str[input_idx*3] << 8) + input_str[input_idx*3+1];
            result_buffer[chunk_idx*4] = (uchar)(input_str[input_idx*3] >> 2);
            result_buffer[chunk_idx*4+1] = (uchar)( (uchar)(sh1 << 6) >> 10);
            result_buffer[chunk_idx*4+2] = (uchar)(input_str[input_idx*3+1] << 4) >> 2;
            result_buffer[chunk_idx*4+3] = '=';
        } else {    // input_length % 3 == 1
            result_buffer[chunk_idx*4] = (uchar)(input_str[input_idx*3] >> 2);
            result_buffer[chunk_idx*4+1] = (uchar)(input_str[input_idx*3+1] << 6) >> 2;
            result_buffer[chunk_idx*4+2] = '=';
            result_buffer[chunk_idx*4+3] = '=';
        }

        input_idx++;
    }

    result_buffer[buffer_len] = '\0';
                              
    return result_buffer;
}

void bitshift_test1()
{
    uchar ch1 = 128;
    uchar ch2 = 255;
    ushort sh1 = (ch1 << 8) + ch2;

    printf("sh1 = %u\n", sh1);
}

void bitshift_test2()
{
    // First char
    uchar ch1 = 'M';
    printf("ch1 = %u\n", ch1);
    printf("ch1 shifted = %u\n", (uchar)(ch1 >> 2));
}

void bitshift_test3()
{
    // last char
    uchar ch1 = 'n';
    // printf("ch1 = %u\n", ch1);
    uchar shifted_ch1 = ch1 << 2;
    shifted_ch1 = shifted_ch1 >> 2;
    printf("shifted_ch1 = %u\n", shifted_ch1);
}

void bitshift_test4()
{
    // Second char
    uchar ch1 = 'M';
    uchar ch2 = 'a';

    ushort sh1 = (ch1 << 8) + ch2;
    sh1 = sh1 << 6;
    sh1 = sh1 >> 10;
    uchar result = sh1;

    printf("bitshift_test4 result = %u\n", result);
}

void bitshift_test5()
{
    // Third char
    uchar ch1 = 'a';
    uchar ch2 = 'n';

    ushort sh1 = (ch1 << 8) + ch2;
    sh1 = sh1 << 4;
    sh1 = sh1 >> 10;
    uchar result = sh1;

    printf("bitshift_test5 result = %u\n", result);
}

int main(void)
{
    uchar ch = 'a';
    uchar ch_arr[] = {'h', 'e', 'l', 'l', 'o'};
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
    uchar *some_str2 = "hello";

    printf("%u %u%u\n", ch, ch_arr[0], ch_arr[1]);
    printf("%c %c%c\n", ch, ch_arr[0], ch_arr[1]);
    printf("%s\n", lorem);
    printf("%u %u %u\n", lorem[1], lorem[10], lorem[11]);
    printf("Lorem len = %d\n", strlen(lorem));
    printf("some_str2 len = %d\n", strlen(some_str2));
    // printf("%d\n", convert_to_base64(some_str, strlen(some_str)));

    uchar ch2 = 129;
    if ((uchar)(ch2 << 1) == 2)
        printf("bingo!\n");
 
    bitshift_test1();
    bitshift_test2();
    bitshift_test3();
    bitshift_test4();
    bitshift_test5();

    uchar *result_base64 = convert_to_base64(lorem, strlen(lorem));
    printf("converted to base64 lorem: %s\n", result_base64);
    free(result_base64);

    return 0;
}