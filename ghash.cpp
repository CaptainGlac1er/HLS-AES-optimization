#include "ghash.h"
#include "debug.h"
#include "aes.h"
#include "gfstuff.h"
#include <string.h>
#include "gcm_methods.h"

void init_ghash_aad(unsigned char H[16], unsigned char aad[1024], unsigned long long aad_len, unsigned char tag[16]){
    unsigned int i,j;
	unsigned char subtext[16];
	for (i=0; i<16; i++) {
#pragma HLS unroll
		tag[i] = 0;
	}

	for(i = 0; i < 1024; i+=16){
#pragma HLS UNROLL region
//#pragma HLS pipeline II=28
        for(j = 0; j < 16; j++){
            if(i + j < aad_len){
            subtext[j] = aad[j+i];
            }else{
                subtext[j] = 0;
            }
        }
		gf_xor(subtext,tag);
		gf_mult_128(H,subtext,tag);
		if (i >= aad_len-1) {
			break;
		}
	}
}
void init_ghash_cycle(unsigned char H[16], unsigned char C[1024], unsigned long C_len, unsigned char tag[16]){
    unsigned int j;
	unsigned char subtext[16];
    memset(subtext, 0, sizeof(subtext));
    for(j = 0; j < C_len; j++){
        subtext[j] = C[j];
    }
    gf_xor(subtext,tag);
    gf_mult(H,subtext,tag);
}
void end_ghash_cycle(unsigned char *H, unsigned long long aad_len, unsigned long C_len, unsigned char *tag){
	unsigned char temp[16]; 
    ConstructArray(temp, aad_len * 8);
    ConstructArray(&temp[8], C_len * 8);
    gf_xor(temp,tag);
    gf_mult(temp,H,tag);
}
