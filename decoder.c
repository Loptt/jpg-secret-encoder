#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jpg_lib.h"

int main(int argc, char** argv) {
    unsigned char buffer[MAX_BUFFER];
    unsigned char modified_buffer[MAX_BUFFER];
    unsigned char *message;
    int message_length;
    int position;
    FILE *fp;

    if (argc < 4) {
        printf("Usage: %s <file> <length> <position>\n", argv[0]);
        return -1;
    }

    fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        printf("Failed to open encoded.jpeg\n");
        return -1;
    }

    message_length = atoi(argv[2]);
    position = atoi(argv[3]);

    // Clear buffer before reading again.
    memset(buffer, 0, MAX_BUFFER);

    int file_size = fread(buffer, 1, MAX_BUFFER, fp);
    fclose(fp);

    unsigned char* decoded_message = malloc(message_length);

    decode_data(decoded_message, buffer, position, message_length);

    printf("Got message: %s\n", decoded_message);

    printf("Done\n");
    return 0;
}