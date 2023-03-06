#define MAX_BUFFER 80000
#define BASE 'A'

int parse_header(char *buffer, int size);

int base_ascii(unsigned char *data, int length, unsigned char base);

int debase_ascii(unsigned char *data, int length, unsigned char base);

int encode_data(unsigned char *buffer, int position, unsigned char *message);

int decode_data(unsigned char* dest, unsigned char* src, int position, int length);