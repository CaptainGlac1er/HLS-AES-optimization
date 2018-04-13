#include "stdio.h"
#include "string.h"
#include "aes.h"
#include "ghash.h"
#include "debug.h"

/*
unsigned char Key[16]        =
{   0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

unsigned char IV[12]        =
{   0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

unsigned char PlainText[16]  =
{   0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};*/

/*unsigned char Key[16] =
{
    0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c, 0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08
}*/

//unsigned char IV

unsigned char Key[16]        =
{0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c, 0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08};

unsigned char IV[12]        =
{0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad, 0xde, 0xca, 0xf8, 0x88};

unsigned char PlainText[64]  =
{   0xd9, 0x31, 0x32, 0x25, 0xf8, 0x84, 0x06, 0xe5, 0xa5, 0x59, 0x09, 0xc5, 0xaf, 0xf5, 0x26, 0x9a,
    0x86, 0xa7, 0xa9, 0x53, 0x15, 0x34, 0xf7, 0xda, 0x2e, 0x4c, 0x30, 0x3d, 0x8a, 0x31, 0x8a, 0x72,
    0x1c, 0x3c, 0x0c, 0x95, 0x95, 0x68, 0x09, 0x53, 0x2f, 0xcf, 0x0e, 0x24, 0x49, 0xa6, 0xb5, 0x25,
    0xb1, 0x6a, 0xed, 0xf5, 0xaa, 0x0d, 0xe6, 0x57, 0xba, 0x63, 0x7b, 0x39, 0x1a, 0xaf, 0xd2, 0x55
};

unsigned char X1[16] = {0x03, 0x88, 0xda, 0xce, 0x60, 0xb6, 0xa3, 0x92, 0xf3, 0x28, 0xc2, 0xb9, 0x71, 0xb2, 0xfe, 0x78};
unsigned char Y1[16] = {0x66, 0xe9, 0x4b, 0xd4, 0xef, 0x8a, 0x2c, 0x3b, 0x88, 0x4c, 0xfa, 0x59, 0xca, 0x34, 0x2b, 0x2e};
unsigned char Z1[16] = {0x5e, 0x2e, 0xc7, 0x46, 0x91, 0x70, 0x62, 0x88, 0x2c, 0x85, 0xb0, 0x68, 0x53, 0x53, 0xde, 0xb7};

unsigned char X2[16] = {0x5e, 0x2e, 0xc7, 0x46, 0x91, 0x70, 0x62, 0x88, 0x2c, 0x85, 0xb0, 0x68, 0x53, 0x53, 0xde, 0x37};
unsigned char Y2[16] = {0x66, 0xe9, 0x4b, 0xd4, 0xef, 0x8a, 0x2c, 0x3b, 0x88, 0x4c, 0xfa, 0x59, 0xca, 0x34, 0x2b, 0x2e};
unsigned char Z2[16] = {0xf3, 0x8c, 0xbb, 0x1a, 0xd6, 0x92, 0x23, 0xdc, 0xc3, 0x45, 0x7a, 0xe5, 0xb6, 0xb0, 0xf8, 0x85};

unsigned char X3[16] = {0xba, 0x47, 0x1e, 0x04, 0x9d, 0xa2, 0x0e, 0x40, 0x49, 0x5e, 0x28, 0xe5, 0x8c, 0xa8, 0xc5, 0x55};
unsigned char Y3[16] = {0xb8, 0x3b, 0x53, 0x37, 0x08, 0xbf, 0x53, 0x5d, 0x0a, 0xa6, 0xe5, 0x29, 0x80, 0xd5, 0x3b, 0x78};
unsigned char Z3[16] = {0xb7, 0x14, 0xc9, 0x04, 0x83, 0x89, 0xaf, 0xd9, 0xf9, 0xbc, 0x5c, 0x1d, 0x43, 0x78, 0xe0, 0x52};






int main() {

    unsigned char OUT[16];

    /*puts("GF MULT TEST 1");
    gf_mult(X1, Y1, OUT);
    AES_PRINT(OUT);
    AES_PRINT(Z1);

    puts("");
    puts("GF MULT TEST 2");
    gf_mult(X2, Y2, OUT);
    AES_PRINT(OUT);
    AES_PRINT(Z2);

    puts("");
    puts("GF MULT TEST 3");
    gf_mult(X3, Y3, OUT);
    AES_PRINT(OUT);
    AES_PRINT(Z3);*/

    puts("");
    puts("ENCRYPTION TEST");
    unsigned char aad[64];
    unsigned char Tag[16];
    unsigned char CipherText[64];
    memset(aad, 0, 64);
    g_counter_mode_encrypt_and_authenticate(Key, IV, PlainText, sizeof (PlainText),
                                            aad, sizeof (aad), CipherText, Tag);
    printf("CIPHER - ");
    AES_PRINT(CipherText);
    printf("CIPHER - ");
    AES_PRINT(CipherText+16);
    printf("CIPHER - ");
    AES_PRINT(CipherText+32);
    printf("CIPHER - ");
    AES_PRINT(CipherText+48);
    return 0;
}
