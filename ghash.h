void init_ghash_aad(unsigned char *H, unsigned char *aad, unsigned long long aad_len, unsigned char *tag);
void init_ghash_cycle(unsigned char *H, unsigned char *C, unsigned long C_len, unsigned char *tag);
void end_ghash_cycle(unsigned char *H, unsigned long long aad_len, unsigned long C_len, unsigned char *tag);

