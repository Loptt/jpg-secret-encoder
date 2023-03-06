#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jpg_lib.h"

int main(int argc, char** argv) {
    unsigned char buffer[MAX_BUFFER];
    unsigned char modified_buffer[MAX_BUFFER];
    unsigned char *message;
    int message_length;
    FILE *fp;

    if (argc < 3) {
        printf("Usage: %s <file> <message>\n", argv[0]);
        return -1;
    }

    message = argv[2];
    message_length = strlen(message);

    fp = fopen(argv[1], "rb");

    if (fp == NULL) {
        printf("Cannot open file %s", argv[1]);
        return -1;
    }

    int file_size = fread(buffer, 1, MAX_BUFFER, fp);
    fclose(fp);

    if (file_size == 0) {
        printf("Cannot read file\n");
        return -1;
    }

    printf("File size: %d\n", file_size);

    int data_start = parse_header(buffer, file_size) + 0x2;
    if (data_start < 0) {
        printf("Failed parsing header\n");
        return -1;
    }

    printf("Data start pointer: 0x%X, data at location: 0x%X\n", data_start, buffer[data_start]);

    memcpy(modified_buffer, buffer, MAX_BUFFER);

    printf("Initialized modified buffer\n");

    if (encode_data(modified_buffer, data_start, message)) {
        printf("Failed to encode data");
        return -1;
    }

    printf("Encoded data successfully\n");

    fp = fopen("encoded.jpeg", "wb");
    if (fp == NULL) {
        printf("Cannot open file encoded.jpeg\n");
        return -1;
    }

    if (fwrite(modified_buffer, 1, file_size, fp) != file_size) {
        printf("Failed to write encoded file\n");
        return -1;
    }

    fclose(fp);
    printf("Succesfully written encoded file\n");

    printf("Done\n");
    return 0;
}