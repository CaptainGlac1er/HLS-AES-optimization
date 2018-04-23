#include "gcm_decrypt.h"
#include "gcm_methods.h"
#include "gfstuff.h"
#include "ghash.h"
#include "aes.h"
#include <string.h>
void decryptCycle(unsigned char key[16], unsigned char H[16], unsigned char H_key[16], unsigned char block[16], unsigned long long b_length, unsigned char out[16], unsigned char X[16]){
	inc32(&(H[12]));
    //encrypt the iv+count
    encrypt(H, key, out);
    //then xor the output with the plaintext to get the cipher text
    gf_xor(out, block);
    init_ghash_cycle(H_key, block, b_length,X);
}
void gcm_decrypt_and_authenticate(unsigned char *key, unsigned char *iv, unsigned char *plaintext, unsigned long long plaintext_length,
    unsigned char *aad, unsigned long long aad_len, unsigned char *ciphertext, unsigned char *tag){
    unsigned int i, j;
    unsigned int blocks = plaintext_length / 16;
    unsigned char H_key[16]; // the hash key
    unsigned char H[16]; // the iv+counter that we encrypt
    unsigned char X[16]; // hash input (A, C, len(A), len(c)

    // get the hash key by encryptin all 0's with the normal key
    init_hash_key(key, H_key);

    // initialize H, the iv+counter
    init_j(iv, H);
    encrypt(H, key, tag);
    
/**************************************************/
    init_ghash_aad(H_key, aad, aad_len,X);
    for (i = 0; i < blocks; i++) {
        //increment the counter;
    	decryptCycle(key, H, H_key, &ciphertext[i * 16], 16, &plaintext[i * 16], X);
    }
    if(plaintext_length > blocks * 16){
        unsigned char extend[16];
        //memcpy(extend, &ciphertext[(blocks) * 16], (plaintext_length - blocks*16));
        for(j = 0; j < 16; j++){
        	extend[j] = 0;
        }
        for(j = 0; j < plaintext_length%16; j++){
        	extend[j] = ciphertext[blocks*16 + j];
        }
        //memset(&extend[plaintext_length - blocks*16], 0,16 - (plaintext_length - blocks*16));
        decryptCycle(key, H, H_key, extend, (plaintext_length - blocks*16), &plaintext[blocks * 16], X);
        //increment the counter
        //inc32(&(H[12]));
        //encrypt the iv+count
        //encrypt(H, key, &(plaintext[blocks*16]));
        //then xor the output with the plaintext to get the cipher text
        //gf_xor(&(plaintext[blocks*16]), extend);
        //init_ghash_cycle(H_key, &ciphertext[blocks*16], (plaintext_length - blocks*16), X);
    }
    end_ghash_cycle(H_key, aad_len, plaintext_length, X);
    gf_xor(tag, X); //final tag step
}
