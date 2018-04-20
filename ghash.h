#include "stdio.h"
#include "string.h"
#include "aes.h"

void gf_mult(const unsigned char *x, const unsigned char *y, unsigned char *z);

void gcm_encrypt_and_authenticate(unsigned char *key, unsigned char *iv, unsigned char *plaintext, unsigned long long plaintext_length,
        unsigned char *aad, unsigned long long aad_len, unsigned char *ciphertext, unsigned char *tag);