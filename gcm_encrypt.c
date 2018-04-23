#include "gcm_encrypt.h"
#include "gcm_methods.h"
#include "gfstuff.h"
#include "ghash.h"
#include "aes.h"
#include <string.h>


void gcm_encrypt_and_authenticate(unsigned char key[16], unsigned char iv[12], unsigned char plaintext[1024], unsigned long long plaintext_length,
        unsigned char aad[1024], unsigned long long aad_len, unsigned char ciphertext[1024], unsigned char tag[16]) {   unsigned int i,j;
    unsigned int blocks = plaintext_length / 16;
    unsigned char H_key[16]; // the hash key
    unsigned char H[16]; // the iv+counter that we encrypt
    unsigned char X[16]; // hash input (A, C, len(A), len(c)
    unsigned char temp[16];

    // get the hash key by encryptin all 0's with the normal key
    init_hash_key(key, H_key);

    // initialize H, the iv+counter
    init_j(iv, H);
    encrypt(H, key, tag);
    
/**************************************************/
    init_ghash_aad(H_key, aad, aad_len,X);
    for (i = 0; i < blocks; i++) {
        //increment the counter
        inc32(&(H[12]));
        //encrypt the iv+count
        encrypt(H, key, &(ciphertext[i*16]));
        //then xor the output with the plaintext to get the cipher text
        gf_xor(&(ciphertext[i*16]), &(plaintext[i*16]));
        for(j = 0; j < 16; j++){
        	temp[j] = ciphertext[i*16 + j];
        }
        init_ghash_cycle(H_key, temp, 16,X);
    }
    if(plaintext_length%16 !=0){

        unsigned char extend[16];
        for(j = 0; j < 16; j++){
        	extend[j] = 0;
        }
        for(j = 0; j < (plaintext_length%16); j++){
        	extend[j] = plaintext[((blocks) * 16) + j];
        }
        //increment the counter
        inc32(&(H[12]));
        //encrypt the iv+count
        encrypt(H, key, &(ciphertext[(blocks)*16]));
        //then xor the output with the plaintext to get the cipher text
        gf_xor(&(ciphertext[(blocks)*16]), extend);
        for(j = 0; j < 16; j++){
        	temp[j] = ciphertext[(blocks)*16 + j];
        }
        init_ghash_cycle(H_key, temp, (plaintext_length - blocks*16),X);
        //init_ghash_cycle(H_key, &ciphertext[i*16], (plaintext_length - blocks*16), X);
    }
    end_ghash_cycle(H_key, aad_len, plaintext_length, X);
    gf_xor(tag, X); //final tag step
}
