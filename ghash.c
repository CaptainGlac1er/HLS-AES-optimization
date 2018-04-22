#include "ghash.h"
#include "debug.h"
#include "aes.h"
#include "gfstuff.h"
#include <string.h>
#include "gcm_methods.h"

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
#pragma HLS unroll
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
