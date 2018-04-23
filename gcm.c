#include "string.h"
#include "gcm.h"

unsigned char TboxCalculated = 0;
unsigned int T[4][256];
const unsigned char SBox[256] = {
 // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,   //0
 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,   //1
 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,   //2
 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,   //3
 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,   //4
 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,   //5
 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,   //6
 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,   //7
 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,   //8
 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,   //9
 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,   //A
 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,   //B
 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,   //C
 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,   //D
 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,   //E
 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }; //F
const unsigned char RCon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};


#define get(x,y)	((unsigned char)(x >> (y*8)))
#define to(x,y)		(((unsigned char)x) << (y*8))
#define xTime(x) ((x<<1) ^ ((x & 0x080) ? 0x1b : 0x00))
void SubBytes (unsigned char StateArray[4][4])
{
	int i,j;
	for(i=0; i<4; i++)
#pragma HLS unroll
		for(j=0; j<4; j++){
#pragma HLS unroll
			StateArray[i][j] = SBox[StateArray[i][j]];
		}
}
void calcTbox(){
    unsigned int i;
	for(i = 0; i <= 0xFF; i++){
#pragma HLS pipeline

		T[0][i] = to(xTime(SBox[i]),0) | to(SBox[i],1) | to(SBox[i],2) |to( xTime(SBox[i])^SBox[i],3) ;

		T[1][i] = to(xTime(SBox[i])^SBox[i],0) | to(xTime(SBox[i]),1) | to(SBox[i],2) | to(SBox[i],3) ;

		T[2][i] = to(SBox[i],0) |  to(xTime(SBox[i])^SBox[i],1) | to(xTime(SBox[i]),2) | to(SBox[i],3) ;

		T[3][i] = to(SBox[i],0) |  to(SBox[i],1) | to(xTime(SBox[i])^SBox[i],2) | to(xTime(SBox[i]),3);
	}
}
void AESRound(unsigned char StateArray[4][4], unsigned char ExpandKey[4][4]){
#pragma HLS ARRAY_PARTITION variable=T
	unsigned int col[4];
#pragma HLS ARRAY_RESHAPE variable=col complete dim=1
	unsigned int expand,box;
    int i,j;
	for(i = 0; i < 4; i++){
#pragma HLS unroll
		expand = ((to(ExpandKey[0][i],0) | to(ExpandKey[1][i],1) | to(ExpandKey[2][i],2) | to(ExpandKey[3][i],3)));
		box = T[0][StateArray[0][i%4]] ^ T[1][StateArray[1][(i+1)%4]] ^ T[2][StateArray[2][(i+2)%4]] ^ T[3][StateArray[3][(i+3)%4]];
		col[i] = expand ^ box;
	}
	for(i = 0; i < 4; i++){
#pragma HLS unroll
		for(j = 0; j < 4; j++){
#pragma HLS unroll
			//30 03
			StateArray[i][j] = get(col[j],i);
		}
	}

}
void ExpandKey (unsigned char Key[4][4], unsigned char ExpandedKey[11][4][4]){
#pragma	ARRAY_PARTITION variable=ExpandedKey
	unsigned char TempKey[4][4];
	memset(TempKey, 0, 4*4*sizeof (unsigned char));
	unsigned char TempKeyCol[4];
#pragma	ARRAY_PARTITION variable=TempKeyCol
	memset(TempKeyCol, 0, 4*sizeof (unsigned char));
	int i,j;

	// Encryption Key copied to Expanded Key [0]
#pragma HLS unroll
	for(i = 0; i < 4; i++){
#pragma HLS unroll
		for(j = 0; j < 4; j++){
			ExpandedKey[0][i][j] = Key[i][j];
		}
	}
	//memcpy(ExpandedKey[0], Key, 4 * 4 * sizeof(unsigned char));

//#pragma HLS pipeline
	for (i=1; i<11; i++){
		// W3 copied to TempKeyRow with rotation
		TempKeyCol[0]=ExpandedKey[i-1][1][3];
		TempKeyCol[1]=ExpandedKey[i-1][2][3];
		TempKeyCol[2]=ExpandedKey[i-1][3][3];
		TempKeyCol[3]=ExpandedKey[i-1][0][3];

		// sBox applied
		TempKeyCol[0]=SBox[ TempKeyCol[0] ];
		TempKeyCol[1]=SBox[ TempKeyCol[1] ];
		TempKeyCol[2]=SBox[ TempKeyCol[2] ];
		TempKeyCol[3]=SBox[ TempKeyCol[3] ];

		// Rcon applied
		TempKeyCol[0]^=RCon[i-1];

		// XOR
#pragma HLS unroll
		for(j=0; j<4; j++){
			TempKeyCol[0] = TempKeyCol[0]^ExpandedKey[i-1][0][j];
			TempKeyCol[1] = TempKeyCol[1]^ExpandedKey[i-1][1][j];
			TempKeyCol[2] = TempKeyCol[2]^ExpandedKey[i-1][2][j];
			TempKeyCol[3] = TempKeyCol[3]^ExpandedKey[i-1][3][j];

			ExpandedKey[i][0][j] = TempKeyCol[0];
			ExpandedKey[i][1][j] = TempKeyCol[1];
			ExpandedKey[i][2][j] = TempKeyCol[2];
			ExpandedKey[i][3][j] = TempKeyCol[3];
		}
	}
}

void AddRoundKey (unsigned char Key[4][4], unsigned char StateArray[4][4])
{
	int i,j;
	for(i=0; i<4; i++){
#pragma HLS unroll
		for(j=0; j<4; j++){
#pragma HLS unroll
			StateArray[i][j] ^= Key[i][j];
		}
	}
}
void ShiftRows (unsigned char StateArray[4][4])
{
	unsigned char x;
	// Row#1 - rotate 1 column to the left
	x = StateArray[1][0];
	StateArray[1][0] = StateArray[1][1];
	StateArray[1][1] = StateArray[1][2];
	StateArray[1][2] = StateArray[1][3];
	StateArray[1][3] = x;
	// Row#2 - rotate 2 column to the left
	x = StateArray[2][0];
	StateArray[2][0] = StateArray[2][2];
	StateArray[2][2] = x;
	x = StateArray[2][1];
	StateArray[2][1] = StateArray[2][3];
	StateArray[2][3] = x;
	// Row#3 - rotate 3 column to the left
	x = StateArray[3][3];
	StateArray[3][3] = StateArray[3][2];
	StateArray[3][2] = StateArray[3][1];
	StateArray[3][1] = StateArray[3][0];
	StateArray[3][0] = x;
}



void encrypt(unsigned char PlainText[16], unsigned char Key[16], unsigned char CipherText[16]){
#pragma HLS ARRAY_PARTITION variable=Key
#pragma HLS ARRAY_PARTITION variable=PlainText
    if(!TboxCalculated){
        calcTbox();
        TboxCalculated = 1;
    }
    unsigned char StateArray[4][4];
    unsigned char TempKey[4][4];
    unsigned char ExpandedKey[11][4][4];
    unsigned int i,j;
    for(i=0; i<4; i++) {
#pragma HLS unroll
        for(j=0; j<4; j++) {
#pragma HLS unroll
            TempKey[i][j] = Key[j*4+i];
            StateArray[i][j] = PlainText[j*4+i];
        }
    }

	ExpandKey(TempKey, ExpandedKey);
    AddRoundKey(ExpandedKey[0], StateArray);
    
    for(i=1; i<=10; i++){
        if(i != 10){
            AESRound(StateArray, ExpandedKey[i]);
        }
        else{
            SubBytes(StateArray);
            ShiftRows(StateArray);
            AddRoundKey(ExpandedKey[i], StateArray);
        }
    }
    for(i=0; i<4; i++) {
#pragma HLS unroll
        for(j=0; j<4; j++) {
#pragma HLS unroll
            CipherText[j*4+i] = StateArray[i][j];
        }
    }
}

void AES_PRINT(unsigned char * array_ptr) {
    printf("HEX:");
    int i;
    for (i=0; i<16; i++) {
        printf(" 0x%02x", array_ptr[i]);
    }
    printf("\r\n");
}

#include <string.h>
void gcm_decrypt_and_authenticate(unsigned char *key, unsigned char *iv, unsigned char *plaintext, unsigned long long plaintext_length,
    unsigned char *aad, unsigned long long aad_len, unsigned char *ciphertext, unsigned char *tag){
    unsigned int i, j, k;
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
        inc32(H);
        //encrypt the iv+count
        encrypt(H, key, &(plaintext[i*16]));
        //then xor the output with the plaintext to get the cipher text
        
        gf_xor(&(plaintext[i*16]), &(ciphertext[i*16]));
        init_ghash_cycle(H_key, &ciphertext[i*16], 16,X);
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
        //increment the counter
        inc32(H);
        //encrypt the iv+count
        encrypt(H, key, &(plaintext[blocks*16]));
        //then xor the output with the plaintext to get the cipher text
        gf_xor(&(plaintext[blocks*16]), extend);
        init_ghash_cycle(H_key, &ciphertext[blocks*16], (plaintext_length - blocks*16), X);
    }
    end_ghash_cycle(H_key, aad_len, plaintext_length, X);
    gf_xor(tag, X); //final tag step
}



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
        inc32(H);
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
        inc32(H);
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


void inc32(unsigned char v[16])
{
    //convert it to an int and then increment it
    int temp = (v[12] << 24) | (v[13] << 16) | (v[14] << 8) | v[15];
    temp++;
    v[15] = (temp >>  0) & 0xff;
    v[14] = (temp >>  8) & 0xff;
    v[13] = (temp >> 16) & 0xff;
    v[12] = (temp >> 24) & 0xff;
}

void init_hash_key(unsigned char key[16], unsigned char H[16])
{
    //hash key is just the encryption of all zeros
    unsigned char temp[16];
    int i;
    for (i = 0; i < 16; i++) {
#pragma HLS unroll
        temp[i] = 0; // set H to zero
    }
    encrypt(temp, key, H);
}

void init_j(unsigned char iv[12], unsigned char H[16])
{
	int i;
    //set the first 96 bits to iv
    for (i=0; i<12; i++) {
#pragma HLS unroll
        H[i] = iv[i];
    }
    // then init the counter with 1
    H[12] = 0x00;
    H[13] = 0x00;
    H[14] = 0x00;
    H[15] = 0x01;
}

void ConstructArray(unsigned char a[8], unsigned long long val)
{
#pragma HLS unroll
	a[0] = val >> 56;
	a[1] = val >> 48;
	a[2] = val >> 40;
	a[3] = val >> 32;
	a[4] = val >> 24;
	a[5] = val >> 16;
	a[6] = val >> 8;
	a[7] = val & 0xff;
}
void gf_rightshift(unsigned char v[16])
{
    //right to left
	int i;
    for (i = 15; i > 0; i--) {
        v[i] = (v[i] >> 1) | (v[i-1] << 7);
    }
    v[0] = (v[0] >> 1);
}
/**
 * z first array
 * v second array
 * z is output 
 */
void gf_xor(unsigned char z[16], unsigned char v[16])
{
	int i;
    for (i = 0; i < 16; i++) {
        z[i] = z[i] ^ v[i];
    }
}

void gf_mult(const unsigned char x[16], const unsigned char y[16], unsigned char z[16])
{
    //FOLLOWING STRAIGHT FROM THE PSEUDOCODE ON PAGE 9
    unsigned char v[16];
    int i,j;

    for (i = 0; i < 16; i++) {
        z[i] = 0; // set z to zero
        v[i] = x[i]; // set v to x
    }

    //FOR I=0 TO 127 DO
    // says 0 to 127 but were chunking into bytes
    for (i = 0; i < 16; i++) {
        // we're going left to right so 7 to 0
        for (j = 7; j >= 0; j--) {
            //IF Y_I = 1 THEN
            if (y[i] & 1 << j) {
                //Z <-- Z XOR V
                gf_xor(z, v);
            }
            //END IF

            //IF V_127 = 0 THEN
            // again left to right so
            if ((v[15] & 0x01) == 0) {
                //V <-- RIGHTSHIFT(V)
                gf_rightshift(v);
            } else {
                //V <-- RIGHTSHIFT(V) XOR R
                gf_rightshift(v);
                // R = 1 + a + a^2 + a^7 + a^128
                // but a^128 is out of the field so
                // just need to xor v[0] with  1 + a + a^2 + a^7
                // see page 10 for details
                v[0] = v[0] ^ 0xe1;
            }
            //END IF



        }
    }
}

void gf_mult_64(const unsigned char x[16], const unsigned char y[16], unsigned char z[16])
{
//#pragma HLS inline
    //FOLLOWING STRAIGHT FROM THE PSEUDOCODE ON PAGE 9
    int i,j;
    unsigned long long my_ap_v64[2];
    unsigned long long my_ap_y64[2];
    unsigned long long my_ap_z64[2];

    my_ap_v64[0] = 0;
    my_ap_y64[0] = 0;
    my_ap_z64[0] = 0;
    my_ap_v64[1] = 0;
    my_ap_y64[1] = 0;
    my_ap_z64[1] = 0;


        for (i = 0; i < 8; i++) {
    #pragma HLS unroll
            my_ap_v64[0] += (unsigned long long)(((unsigned long long)x[i]) << (8*(7-i))); // set my_ap_v64 to x
            my_ap_y64[0] += (unsigned long long)(((unsigned long long)y[i]) << (8*(7-i))); // set my_ap_y64 to y
            my_ap_v64[1] += (unsigned long long)(((unsigned long long)x[i+8]) << (8*(7-i))); // set my_ap_v64 to x
            my_ap_y64[1] += (unsigned long long)(((unsigned long long)y[i+8]) << (8*(7-i))); // set my_ap_y64 to y
        }

    //FOR I=0 TO 127 DO
    // says 0 to 127 but were chunking into bytes
    for (i = 0; i < 2; i++) {
#pragma HLS unroll region
        // we're going left to right so 63 to 0
        for (j = 63; j >= 0; j--) {
            //IF Y_I = 1 THEN
            if (my_ap_y64[i] & (unsigned long long) 1 << j) {
                //Z <-- Z XOR V
                my_ap_z64[0] = my_ap_z64[0] ^ my_ap_v64[0];
                my_ap_z64[1] = my_ap_z64[1] ^ my_ap_v64[1];
            }
            //END IF

            //IF V_127 = 0 THEN
            // again left to right so
            if ((my_ap_v64[1] & (unsigned long long)0x01) == 0) {
                //V <-- RIGHTSHIFT(V)
                my_ap_v64[1] = (my_ap_v64[1] >> 1) | (my_ap_v64[0] << 63);
                my_ap_v64[0] = my_ap_v64[0] >> 1;
            } else {
                //V <-- RIGHTSHIFT(V) XOR R
                my_ap_v64[1] = (my_ap_v64[1] >> 1) | (my_ap_v64[0] << 63);
                my_ap_v64[0] = my_ap_v64[0] >> 1;
                // R = 1 + a + a^2 + a^7 + a^128
                // but a^128 is out of the field so
                // just need to xor v[0] with  1 + a + a^2 + a^7
                // see page 10 for details
                my_ap_v64[0] = my_ap_v64[0] ^ ((unsigned long long)0xe1 << 56);
            }
            //END IF
        }
    }
    for (i = 0; i < 8; i++) {
#pragma HLS unroll
        z[i+8] = (unsigned char) (my_ap_z64[1] >> (8*(7-i)))&0xff; // set z to my_ap_z64
        z[i] = (unsigned char) (my_ap_z64[0] >> (8*(7-i)))&0xff; // set z to my_ap_z64
    }
}


void init_ghash_aad(unsigned char H[16], unsigned char aad[1024], unsigned long long aad_len, unsigned char tag[16]){
    unsigned int i,j;
    for (i = 0 ; i < 16; i++) {
    	tag[i] = 0;
    }
	unsigned char subtext[16];
	for(i = 0; i < 1024; i+=16){
#pragma HLS unroll region
        for(j = 0; j < 16; j++){
            if(i + j < aad_len){
            subtext[j] = aad[j+i];
            }else{
                subtext[j] = 0;
            }
        }
		gf_xor(subtext,tag);
		gf_mult_64(H,subtext,tag);
		if(i+j >= aad_len) {
		    return;
		}
	}
}
void init_ghash_cycle(unsigned char H[16], unsigned char C[16], unsigned long C_len, unsigned char tag[16]){
    unsigned int j;
	unsigned char subtext[16];
    for (j = 0 ; j < 16; j++) {
//#pragma HLS unroll
    	if(j < C_len){
    		subtext[j] = C[j];
    	}
    	else {
    	    subtext[j] = 0;
        }
    }
    gf_xor(subtext,tag);
    gf_mult_64(H,subtext,tag);
}
void end_ghash_cycle(unsigned char H[16], unsigned long long aad_len, unsigned long C_len, unsigned char tag[16]){
	unsigned char temp[16]; 
    ConstructArray(temp, aad_len * 8);
    ConstructArray(&temp[8], C_len * 8);
    gf_xor(temp,tag);
    gf_mult_64(temp,H,tag);
}

#include "stdio.h"
#include <stdlib.h>

void PrintMessage(unsigned char *message, unsigned long length){
    unsigned int i,j;
    printf(" -- Message Start -- \r\n");
    for( i = 0; i < length; i +=16){
        printf("DATA - ");
        for(j = 0; j < 16 && i + j < length; j++){
            printf("%02X ",message[i+j]);
        }
        printf(" [%d] \r\n", j);
    }
    printf(" -- Message End -- \r\n");
}
EncyptedMessage* newEncyptedMessage(unsigned long HeaderLength, unsigned long SeqLength, unsigned long DataLength, unsigned long ICVLength){
    EncyptedMessage* out = malloc(sizeof(EncyptedMessage));
    out->Header = malloc(HeaderLength);
    out->HeaderLength = HeaderLength;
    out->Data = malloc(DataLength);
    out->DataLength = DataLength;
    out->ICV = malloc(ICVLength);
    out->ICVLength = ICVLength;
    out->Seq = malloc(SeqLength);
    out->SeqLength = SeqLength;
    return out;
}
Message* newMessage(unsigned long HeaderLength, unsigned long SeqLength, unsigned long DataLength){
    Message* out = malloc(sizeof(Message));
    out->Header = malloc(HeaderLength);
    out->HeaderLength = HeaderLength;
    out->Data = malloc(DataLength);
    out->DataLength = DataLength;
    out->Seq = malloc(SeqLength);
    out->SeqLength = SeqLength;
    return out;
}
