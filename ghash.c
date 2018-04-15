#include "ghash.h"
#include "debug.h"
#include "aes.h"
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
/**
 * z first array
 * v second array
 * z is output 
 */
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

void init_j(unsigned char *iv, unsigned char *H)
{
	int i;
    //set the first 96 bits to iv
    for (i=0; i<12; i++) {
        H[i] = iv[i];
    }
    // then init the counter with 1
    H[12] = 0x00;
    H[13] = 0x00;
    H[14] = 0x00;
    H[15] = 0x01;
}


void g_counter_mode_encrypt(unsigned char *H, unsigned char *plaintext, size_t plaintext_length, unsigned char key[16], unsigned char *ciphertext)
{
    int i;

    if (plaintext_length == 0)
        return;
    int blocks = plaintext_length / 16;
    printf("blocks: %d %d", blocks, plaintext_length);

    for (i = 0; i < blocks; i++) {
        //increment the counter
        inc32(&(H[12]));
        //encrypt the iv+count
        encrypt(H, key, &(ciphertext[i*16]));
        //then xor the output with the plaintext to get the cipher text
        gf_xor(&(ciphertext[i*16]), &(plaintext[i*16]));
        printf("\r\n%d",i);
    }
    if(plaintext_length > blocks * 16){
        unsigned char extend[16];
        memcpy(extend, &plaintext[(blocks) * 16], (plaintext_length - blocks*16));
        memset(&extend[plaintext_length - blocks*16], 0,16 - (plaintext_length - blocks*16));
        //increment the counter
        inc32(&(H[12]));
        //encrypt the iv+count
        encrypt(H, key, &(ciphertext[i*16]));
        //then xor the output with the plaintext to get the cipher text
        gf_xor(&(ciphertext[i*16]), extend);
    }

    //if theres an unfull block...
    /*int remainder = plaintext_length % 16;
    if (remainder != 0) {
        unsigned char temp[16];
        encrypt(H, key, temp);
        for (i = 0; i < remainder; i++) {
            ciphertext[(blocks-1)*16 + i] =  plaintext[(blocks-1)*16 + i] ^ temp[i];
        }
    }*/
}
void ghash(unsigned char *H, unsigned char *A, unsigned long A_len, unsigned char *C, unsigned long C_len, unsigned char *X, unsigned char *key, unsigned char *iv){
	memset(X, 0, 16);
    unsigned int i,j;
	unsigned char temp[16];
	unsigned char subtext[16];
	unsigned char cur;
    memset(temp, 0, 16);
    i = 0;
	do{
        for(j = 0; j < 16; j++){
            if(i + j < A_len){
            subtext[j] = A[j+i];
            }else{
                subtext[j] = 0;
            }
        }
		gf_xor(subtext,temp);
		gf_mult(H,subtext,temp);
        printf("X - ");
        AES_PRINT(temp);
        i+=16;
	}while(i < A_len);
    
    printf("AX - ");
    AES_PRINT(temp);
	for(i = 0; i < C_len; i+=16){
        printf("cur - ");
        AES_PRINT(&C[i]);
        for(j = 0; j < 16; j++){
            if(i + j < C_len){
                subtext[j] = C[j+i];
            }else{
                subtext[j] = 0;
            }
        }
        AES_PRINT(subtext);
		gf_xor(subtext,temp);
		gf_mult(H,subtext,temp);
        printf("X - ");
        AES_PRINT(temp);
	}
    memcpy(X, temp, 16);
    ConstructArray(temp, A_len * 8);
    ConstructArray(&temp[8], C_len * 8);
    AES_PRINT(temp);
    gf_xor(temp,X);
    gf_mult(temp,H,X);
}

void g_counter_mode_encrypt_and_authenticate(unsigned char *key, unsigned char *iv, unsigned char *plaintext, size_t plaintext_length,
        unsigned char *aad, size_t aad_len, unsigned char *ciphertext, unsigned char *tag)
{
    unsigned char H_key[16]; // the hash key
    unsigned char H[16]; // the iv+counter that we encrypt
    unsigned char X[16]; // hash input (A, C, len(A), len(c)
    printf("KEY - ");
    AES_PRINT(key);

    // get the hash key by encryptin all 0's with the normal key
    init_hash_key(key, H_key);

    printf("H_KEY - ");
    AES_PRINT(H_key);
    // initialize H, the iv+counter
    init_j(iv, H);

    printf("IV - ");
    AES_PRINT(H);
    // for whatever reason increment so the count is 2 before you start
    //inc32(&(H[12]));
    // do the encryption
    printf("P - ");
    AES_PRINT(plaintext);
    g_counter_mode_encrypt(H, plaintext, plaintext_length, key, ciphertext);
    printf("E(K,Y) - ");
    AES_PRINT(ciphertext);
    // do the hash
    printf("H - ");
    AES_PRINT(H_key);

    ghash(H_key, aad, aad_len, ciphertext, plaintext_length, X, key, iv);

    printf("GHASH - ");
    AES_PRINT(X);
    //reset H for the tag
    init_j(iv, H);
    //get the tag
    encrypt(H, key, tag);
    gf_xor(tag, X);
    printf("TAG - ");
    AES_PRINT(tag);
}
