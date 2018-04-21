#include "gcm_encrypt.h"
#include "gcm_methods.h"
#include "gfstuff.h"
#include "ghash.h"
#include "aes.h"
#include <string.h>


void gcm_encrypt_and_authenticate(unsigned char *key, unsigned char *iv, unsigned char *plaintext, unsigned long long plaintext_length,
        unsigned char *aad, unsigned long long aad_len, unsigned char *ciphertext, unsigned char *tag){
    unsigned int i;
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
        //increment the counter
        inc32(&(H[12]));
        //encrypt the iv+count
        encrypt(H, key, &(ciphertext[i*16]));
        //then xor the output with the plaintext to get the cipher text
        gf_xor(&(ciphertext[i*16]), &(plaintext[i*16]));
        init_ghash_cycle(H_key, &ciphertext[i*16], 16,X);
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
        init_ghash_cycle(H_key, &ciphertext[i*16], (plaintext_length - blocks*16), X);
    }
    end_ghash_cycle(H_key, aad_len, plaintext_length, X);
    gf_xor(tag, X); //final tag step
}