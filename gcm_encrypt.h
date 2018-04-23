void gcm_encrypt_and_authenticate(unsigned char key[16], unsigned char iv[12], unsigned char plaintext[1024], unsigned long long plaintext_length,
        unsigned char aad[1024], unsigned long long aad_len, unsigned char ciphertext[1024], unsigned char tag[16]);
