#include "gcm_encrypt.h"
#include "gcm_methods.h"
#include "gfstuff.h"
#include "ghash.h"
#include "aes.h"
#include "debug.h"
#include <string.h>

void encryptCycle(unsigned char key[16], unsigned char H[16], unsigned char H_key[16], unsigned char block[16], unsigned long long b_length, unsigned char out[16], unsigned char X[16]){
//#pragma HLS INTERFACE s_axilite port=return bundle=AESGCM_BUS
//#pragma HLS INTERFACE s_axilite port=key bundle=AESGCM_BUS
//#pragma HLS INTERFACE s_axilite port=H bundle=AESGCM_BUS
//#pragma HLS INTERFACE s_axilite port=H_key bundle=AESGCM_BUS
//#pragma HLS INTERFACE s_axilite port=block bundle=AESGCM_BUS
//#pragma HLS INTERFACE s_axilite port=b_length bundle=AESGCM_BUS
//#pragma HLS INTERFACE s_axilite port=out bundle=AESGCM_BUS
//#pragma HLS INTERFACE s_axilite port=X bundle=AESGCM_BUS
	unsigned int i;
	unsigned char temp[16];
	//increment the counter
	inc32(&(H[12]));
	//encrypt the iv+count
	encrypt(H, key, out);
	//then xor the output with the plaintext to get the cipher text
	gf_xor(out, block);
	for(i = 0; i < b_length; i++){
		temp[i] = out[i];
	}
	//init_ghash_cycle(H_key, temp, 16,X);
    init_ghash_cycle(H_key, temp, b_length,X);
}
void gcm_encrypt_and_authenticate(unsigned char key[16], unsigned char iv[12], unsigned char plaintext[1024], unsigned long long plaintext_length,
        unsigned char aad[1024], unsigned long long aad_len, unsigned char ciphertext[1024], unsigned char tag[16]) {   unsigned int i,j;
#pragma HLS INTERFACE s_axilite port=return bundle=AESGCM_BUS
#pragma HLS INTERFACE s_axilite port=key bundle=AESGCM_BUS
#pragma HLS INTERFACE s_axilite port=iv bundle=AESGCM_BUS
#pragma HLS INTERFACE s_axilite port=plaintext bundle=AESGCM_BUS
#pragma HLS INTERFACE s_axilite port=plaintext_length bundle=AESGCM_BUS
#pragma HLS INTERFACE s_axilite port=aad bundle=AESGCM_BUS
#pragma HLS INTERFACE s_axilite port=aad_len bundle=AESGCM_BUS
#pragma HLS INTERFACE s_axilite port=ciphertext bundle=AESGCM_BUS
#pragma HLS INTERFACE s_axilite port=tag bundle=AESGCM_BUS

    unsigned int blocks = plaintext_length / 16;
    unsigned char H_key[16]; // the hash key
    unsigned char H[16]; // the iv+counter that we encrypt
    unsigned char X[16]; // hash input (A, C, len(A), len(c)
    //unsigned char temp[16];

    // get the hash key by encryptin all 0's with the normal key
    init_hash_key(key, H_key);

    // initialize H, the iv+counter
    init_j(iv, H);
    encrypt(H, key, tag);
    
/**************************************************/
    init_ghash_aad(H_key, aad, aad_len,X);
    for (i = 0; i < blocks; i++) {
    	encryptCycle(key, H, H_key, &plaintext[i * 16], 16, &ciphertext[i * 16], X);
    }
    if(plaintext_length%16 !=0){

        unsigned char extend[16];
        for(j = 0; j < 16; j++){
        	extend[j] = 0;
        }
        for(j = 0; j < (plaintext_length%16); j++){
        	extend[j] = plaintext[((blocks) * 16) + j];
        }
        encryptCycle(key, H, H_key, extend, (plaintext_length - blocks*16), &ciphertext[blocks * 16], X);
    }
    end_ghash_cycle(H_key, aad_len, plaintext_length, X);
    gf_xor(tag, X); //final tag step
}
