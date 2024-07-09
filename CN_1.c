#include <stdio.h>

int main() {
    unsigned short num = 0x3412;  

   
    unsigned char *ptr = (unsigned char *)&num;

    
    if (*ptr == 0x12) {
        printf("Host byte ordering is Little Endian\n");
    } else if (*ptr == 0x34) {
        printf("Host byte ordering is Big Endian\n");
    } else {
        printf("Unable to determine byte ordering\n");
    }

    
    printf("Data in memory:\n");
    int i;
    for ( i = 0; i < sizeof(num); i++) {
        printf("Byte %d: 0x%02X\n", i + 1, ptr[i]);
    }

    return 0;
}

