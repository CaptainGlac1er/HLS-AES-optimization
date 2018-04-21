#include "gcm_methods.h"
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

void ConstructArray(unsigned char *a, unsigned long long val)
{
	a[0] = val >> 56;
	a[1] = val >> 48;
	a[2] = val >> 40;
	a[3] = val >> 32;
	a[4] = val >> 24;
	a[5] = val >> 16;
	a[6] = val >> 8;
	a[7] = val & 0xff;
}