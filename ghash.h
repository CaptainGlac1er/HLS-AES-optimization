void init_ghash_aad(unsigned char H[16], unsigned char aad[1024], unsigned long long aad_len, unsigned char tag[16]);
void init_ghash_cycle(unsigned char H[16], unsigned char C[16], unsigned long C_len, unsigned char tag[16]);
void end_ghash_cycle(unsigned char H[16], unsigned long long aad_len, unsigned long C_len, unsigned char tag[16]);

