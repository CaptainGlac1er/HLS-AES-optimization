#ifndef AES_H
#define AES_H

void encrypt(unsigned char PlainText[16], unsigned char Key[16], unsigned char CipherText[16]);

#endif
#ifndef DEBUG_H
#define DEBUG_H
void AES_PRINT(unsigned char array_ptr[16]);

#endif
#ifndef DECRYPT_H
#define DECRYPT_H

void gcm_decrypt_and_authenticate(unsigned char key[16], unsigned char iv[12], unsigned char plaintext[1024], unsigned long long plaintext_length,
    unsigned char aad[1024], unsigned long long aad_len, unsigned char ciphertext[1024], unsigned char tag[16]);

#endif
#ifndef ENCRYPT
#define ENCRYPT

void gcm_encrypt_and_authenticate(unsigned char key[16], unsigned char iv[12], unsigned char plaintext[1024], unsigned long long plaintext_length,
        unsigned char aad[1024], unsigned long long aad_len, unsigned char ciphertext[1024], unsigned char tag[16]);
#endif
#ifndef GMETHODS
#define GMETHODS

void inc32(unsigned char v[4]);
void init_hash_key(unsigned char key[16], unsigned char H[16]);
void init_j(unsigned char iv[12], unsigned char H[16]);
void ConstructArray(unsigned char a[8], unsigned long long val);

#endif
#ifndef GFSTUFF
#define GFSTUFF

void gf_rightshift(unsigned char v[16]);
void gf_xor(unsigned char z[16], unsigned char v[16]);
void gf_mult(const unsigned char x[16], const unsigned char y[16], unsigned char z[16]);
void gf_mult_64(const unsigned char x[16], const unsigned char y[16], unsigned char z[16]);

#endif
#ifndef GHASH
#define GHASH

void init_ghash_aad(unsigned char H[16], unsigned char aad[1024], unsigned long long aad_len, unsigned char tag[16]);
void init_ghash_cycle(unsigned char H[16], unsigned char C[16], unsigned long C_len, unsigned char tag[16]);
void end_ghash_cycle(unsigned char H[16], unsigned long long aad_len, unsigned long C_len, unsigned char tag[16]);

#endif
#ifndef GUNITS
#define GUNITS

typedef struct Message{
    unsigned char *Header;
    unsigned long HeaderLength;
    unsigned char *Seq;
    unsigned long SeqLength;
    unsigned char *Data;
    unsigned long DataLength;
} Message;

typedef struct EncyptedMessage{
    unsigned char *Header;
    unsigned long HeaderLength;
    unsigned char *Seq;
    unsigned long SeqLength;
    unsigned char *Data;
    unsigned long DataLength;
    unsigned char *ICV;
    unsigned long ICVLength;
} EncyptedMessage;

EncyptedMessage* newEncyptedMessage(unsigned long HeaderLength, unsigned long SeqLength, unsigned long DataLength, unsigned long ICVLength);
Message* newMessage(unsigned long HeaderLength, unsigned long SeqLength, unsigned long DataLength);

void PrintMessage(unsigned char *message, unsigned long length);

#endif
