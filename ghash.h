#include "stdio.h"
#include "string.h"
#include "aes.h"

void g_counter_mode_encrypt_and_authenticate(unsigned char *key, unsigned char *iv, unsigned char *plaintext, size_t plaintext_length,
        unsigned char *aad, size_t aad_len, unsigned char *ciphertext, unsigned char *tag);
void gf_mult(const unsigned char *x, const unsigned char *y, unsigned char *z);