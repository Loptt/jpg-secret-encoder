#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "jpg_lib.h"

int parse_header(char *buffer, int size) {
    int pointer = 0;
    unsigned char curr_value;
    unsigned char marker_id;
    int seg_size;
    int done = 0;

    while (!done && pointer < size) {
        curr_value = buffer[pointer];

        // 0xff indicates the start of a marker
        if (curr_value != 0xff) {
            printf("Not at marker indicator: 0x%X\n", curr_value);
            return -1;
        }

        marker_id = buffer[pointer+1];
        switch (marker_id) {
            case 0xd8:
                printf("0x%X: Start of Image\n", pointer);
                // When it's start of image, immediately skip 2 bytes
                pointer += 2;
                continue;
            case 0xe0:
            case 0xe2:
                printf("0x%X: Application0\n", pointer);
                break;
            case 0xdb:
                printf("0x%X: Quantization Table\n", pointer);
                break;
            case 0xc0:
                printf("0x%X: Start of Frame\n", pointer);
                break;
            case 0xc4:
                printf("0x%X: Huffman Table\n", pointer);
                break;
            case 0xda:
                printf("0x%X: Start of Scan\n", pointer);
                done = 1;
                break;
            default:
                printf("0x%X: Unidentified marker: 0x%X\n", pointer, marker_id);
        }

        // Size of header segment comes in the next two bytes
        seg_size = (buffer[pointer+2] << 8) + buffer[pointer+3];

        // Next segment is current pointer + seg_size and the 2 bytes of the marker.
        pointer += seg_size + 2;
    }

    if (pointer >= size) {
        return -1;
    }

    return pointer;
}

int base_ascii(unsigned char *data, int length, unsigned char base) {
    for (int i = 0; i < length; i++) {
        if (data[i] < base) {
            printf("Byte 0x%X is smaller than base 0x%X", data[i], base);
            return -1;
        }
        char first_data = data[i];
        data[i] = data[i] - base;
    }

    return 0;
}

int debase_ascii(unsigned char *data, int length, unsigned char base) {
    for (int i = 0; i < length; i++) {
        data[i] = data[i] + base;
    }

    return 0;
}

int encode_data(unsigned char *buffer, int position, unsigned char *message) {
    int message_length = strlen(message);
    unsigned char *based_message = malloc(message_length);

    memcpy(based_message, message, message_length);

    if (base_ascii(based_message, message_length, BASE)) {
        printf("Could not base ASCII: %s\n", message);
        free(based_message);
        return -1;
    }

    for (int i = 0; i < message_length; i++) {
        buffer[position + i] += based_message[i];
    }
    
    free(based_message);

    return 0;
}

int decode_data(unsigned char* dest, unsigned char* src, int position, int length) {
    memcpy(dest, src + position, length);
    dest[length] = '\0';
    debase_ascii(dest, length, BASE);

    return 0;
}
