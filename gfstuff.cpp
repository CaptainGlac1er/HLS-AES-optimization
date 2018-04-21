#include "ap_int.h"

void gf_rightshift(unsigned char v[16])
{
//#pragma HLS inline
    //right to left
	int i;
    for (i = 15; i > 0; i--) {
#pragma HLS unroll
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
//#pragma HLS inline
	int i;
    for (i = 0; i < 16; i++) {
#pragma HLS unroll
        z[i] = z[i] ^ v[i];
    }
}

void gf_mult(const unsigned char x[16], const unsigned char y[16], unsigned char z[16])
{
//#pragma HLS inline
    //FOLLOWING STRAIGHT FROM THE PSEUDOCODE ON PAGE 9
    unsigned char v[16];
    int i,j;

    for (i = 0; i < 16; i++) {
#pragma HLS unroll
        z[i] = 0; // set z to zero
        v[i] = x[i]; // set v to x
    }

    //FOR I=0 TO 127 DO
    // says 0 to 127 but were chunking into bytes
    for (i = 0; i < 16; i++) {
//#pragma HLS unroll region
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
            //V <-- RIGHTSHIFT(V)
            gf_rightshift(v);
            if ((v[15] & 0x01) != 0) {
                //V XOR R
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

void gf_mult_128(const unsigned char x[16], const unsigned char y[16], unsigned char z[16])
{
//#pragma HLS inline
    //FOLLOWING STRAIGHT FROM THE PSEUDOCODE ON PAGE 9
    //unsigned char v[16];
    int i,j;

    ap_int<128> v128=0;
    ap_int<128> y128=0;
    ap_int<128> z128=0;

    for (i = 0; i < 16; i++) {
#pragma HLS unroll
        v128 += x[i] << (8*(15-i)); // set v128 to x
        y128 += y[i] << (8*(15-i)); // set y128 to y
    }

    //FOR I=0 TO 127 DO
    for (i = 0; i < 128; i++) {
//#pragma HLS pipeline II=1
        //IF Y_I = 1 THEN
        if (y128 & 1 << i) {
            //Z <-- Z XOR V
        	z128 = z128 ^ v128;
        }
        //END IF

        //IF V_127 = 0 THEN
        //V <-- RIGHTSHIFT(V)
        v128 = v128 >> 1;
        if ((v128 & 0x01) != 0) {
            //V XOR R
            // R = 1 + a + a^2 + a^7 + a^128
            // but a^128 is out of the field so
            // just need to xor v[0] with  1 + a + a^2 + a^7
            // see page 10 for details
            v128 = v128 ^ 0xe1;
        }
        //END IF
    }


    for (i = 0; i < 16; i++) {
#pragma HLS unroll
        z[i] = (z128 >> (8*(15-i)))&0xff; // set z to z128
    }
}
