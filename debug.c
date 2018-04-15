#include "debug.h"
#include "stdio.h"
void AES_PRINT(unsigned char * array_ptr) {
    printf("HEX:");
    int i;
    for (i=0; i<16; i++) {
        printf(" 0x%02x", array_ptr[i]);
    }
    printf("\r\n");
}