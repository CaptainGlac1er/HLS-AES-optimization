#include "ghash.h"
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
/*void ghash(unsigned char *J, unsigned char *text, unsigned int text_length, unsigned char *Y){
	memset(Y, 0, 16);
	unsigned int i;
	unsigned char sub[16];
	printf("%d\r\n",text_length);
	for(i = 0; i < text_length; i+=16){
		memcpy(sub,J,16);
		AES_PRINT(sub);
		gf_xor(sub,Y);
		AES_PRINT(sub);
		gf_mult(sub,&text[i],Y);
		AES_PRINT(Y);
		gf_mult(sub,&text[i],Y);
		inc32(J);
	}
}*/
void ghash(unsigned char *J, unsigned char *text, unsigned int text_length, unsigned char *Y){
	memset(Y, 0, 16);
	unsigned int i;
	unsigned char sub[16];
	printf("%d\r\n",text_length);
	for(i = 0; i < text_length; i+=16){
		memcpy(sub,J,16);
		AES_PRINT(sub);
		gf_xor(sub,Y);
		AES_PRINT(sub);
		gf_mult(sub,&text[i],Y);
		AES_PRINT(Y);
		gf_mult(sub,&text[i],Y);
		inc32(J);
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
    printf("P - ");
    AES_PRINT(plaintext);
    g_counter_mode_encrypt(J, plaintext, plaintext_length, key, ciphertext);
    printf("E(K,Y) - ");
    AES_PRINT(ciphertext);
    // do the hash
    printf("H - ");
    AES_PRINT(H_key);

    ghash(H_key, ciphertext, plaintext_length, X);
    printf("GHASH - ");
    AES_PRINT(X);
    //reset J for the tag
    init_j(iv, J);
    //get the tag
    g_counter_mode_encrypt(J, X, 16, key, tag);
}
