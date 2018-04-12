#include "stdio.h"
#include "string.h"
#include "aes.h"


//unsigned char Key[16]        =
//{   0x00, 0x00, 0x00, 0x00,
//    0x00, 0x00, 0x00, 0x00,
//    0x00, 0x00, 0x00, 0x00,
//    0x00, 0x00, 0x00, 0x00
//};
//
//unsigned char IV[12]        =
//{   0x00, 0x00, 0x00, 0x00,
//    0x00, 0x00, 0x00, 0x00,
//    0x00, 0x00, 0x00, 0x00
//};
//
//unsigned char PlainText[16]  =
//{   0x00, 0x00, 0x00, 0x00,
//    0x00, 0x00, 0x00, 0x00,
//    0x00, 0x00, 0x00, 0x00,
//    0x00, 0x00, 0x00, 0x00
//};

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



void AES_PRINT(unsigned char * array_ptr) {
    printf("HEX:");
    int i;
    for (i=0; i<16; i++) {
        printf(" 0x%02x", array_ptr[i]);
    }
    puts("");
}

void inc32(unsigned char *v)
{
    //convert it to an int and then increment it
    int temp = (v[0] << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    temp++;
    v[3] = (temp >>  0) & 0xff;
    v[2] = (temp >>  8) & 0xff;
    v[1] = (temp >> 16) & 0xff;
    v[0] = (temp >> 24) & 0xff;
}

void gf_rightshift(unsigned char *v)
{
    //right to left
	int i;
    for (i = 15; i > 0; i--) {
        v[i] = (v[i] >> 1) | (v[i-1] << 7);
    }
    v[0] = (v[0] >> 1);
}

void gf_xor(unsigned char *z, unsigned char *v)
{
	int i;
    for (i = 0; i < 16; i++) {
        z[i] = z[i] ^ v[i];
    }
}

void gf_mult(const unsigned char *x, const unsigned char *y, unsigned char *z)
{
    //FOLLOWING STRAIGHT FROM THE PSEUDOCODE ON PAGE 9
    unsigned char v[16];
    int i,j;

    for (i = 0; i < 16; i++) {
        z[i] = 0; // set z to zero
        v[i] = x[i]; // set v to x
    }

    //FOR I=0 TO 127 DO
    // says 0 to 127 but were chunking into bytes
    for (i = 0; i < 16; i++) {
        // we're going left to right so 7 to 0
        for (j = 7; j >= 0; j--) {
            //IF Y_I = 1 THEN
            if (y[i] & 1 << j) {
                //Z <-- Z XOR V
                gf_xor(z, v);
            }
            //END IF

            //IF V_127 = 0 THEN
            // again left to right so
            if ((v[15] & 0x01) == 0) {
                //V <-- RIGHTSHIFT(V)
                gf_rightshift(v);
            } else {
                //V <-- RIGHTSHIFT(V) XOR R
                gf_rightshift(v);
                // R = 1 + a + a^2 + a^7 + a^128
                // but a^128 is out of the field so
                // just need to xor v[0] with  1 + a + a^2 + a^7
                // see page 10 for details
                v[0] = v[0] ^ 0xe1;
            }
            //END IF

        }
        //printf("looping %d %d\r\n",i,j);
    }
    //END FOR
    //RETURN Z
}



void init_hash_key(unsigned char *key, unsigned char *H)
{
    //hash key is just the encryption of all zeros
    unsigned char temp[16];
    int i;
    for (i = 0; i < 16; i++) {
        temp[i] = 0; // set H to zero
    }
    encrypt(temp, key, H);
}

void init_j(unsigned char *iv, unsigned char *J)
{
	int i;
    //set the first 96 bits to iv
    for (i=0; i<12; i++) {
        J[i] = iv[i];
    }
    // then init the counter with 1
    J[12] = 0x00;
    J[13] = 0x00;
    J[14] = 0x00;
    J[15] = 0x01;
}


void g_counter_mode_encrypt(unsigned char *J, unsigned char *plaintext, size_t plaintext_length, unsigned char key[16], unsigned char *ciphertext)
{
    if (plaintext_length == 0)
        return;
    int i;
    int blocks = plaintext_length / 16;

    for (i = 0; i < blocks; i++) {
        //encrypt the iv+count
        encrypt(J, key, &(ciphertext[i*16]));
        //then xor the output with the plaintext to get the cipher text
        gf_xor(&(ciphertext[i*16]), &(plaintext[i*16]));
        //then increment the counter
        inc32(&(J[12]));
    }

    //if theres an unfull block...
    int remainder = plaintext_length % 16;
    if (remainder != 0) {
        unsigned char temp[16];
        encrypt(J, key, temp);
        for (i = 0; i < remainder; i++) {
            ciphertext[(blocks-1)*16 + i] =  plaintext[(blocks-1)*16 + i] ^ temp[i];
        }
    }
}

void g_counter_mode_encrypt_and_authenticate(unsigned char *key, unsigned char *iv, unsigned char *plaintext, size_t plaintext_length,
        unsigned char *aad, size_t aad_len, unsigned char *ciphertext, unsigned char *tag)
{
    unsigned char H_key[16]; // the hash key
    unsigned char J[16]; // the iv+counter that we encrypt
    unsigned char X[16]; // hash input (A, C, len(A), len(c)
    printf("KEY - ");
    AES_PRINT(key);

    // get the hash key by encryptin all 0's with the normal key
    init_hash_key(key, H_key);

    printf("H_KEY - ");
    AES_PRINT(H_key);
    // initialize J, the iv+counter
    init_j(iv, J);

    printf("IV - ");
    AES_PRINT(J);
    // for whatever reason increment so the count is 2 before you start
    inc32(&(J[12]));
    // do the encryption
    g_counter_mode_encrypt(J, plaintext, plaintext_length, key, ciphertext);

    // do the hash
    //ghash(H_key, aad, aad_len, ciphertext, plaintext_length, X);

    //reset J for the tag
    init_j(iv, J);
    //get the tag
    g_counter_mode_encrypt(J, X, 16, key, tag);
}


int main() {

    unsigned char OUT[16];

    puts("GF MULT TEST 1");
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
    AES_PRINT(Z3);

    puts("");
    puts("ENCRYPTION TEST");
    unsigned char aad[1234];
    unsigned char Tag[1234];
    unsigned char CipherText[1234];
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
