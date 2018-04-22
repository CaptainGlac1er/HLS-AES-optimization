void gf_rightshift(unsigned char *v)
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
void gf_xor(unsigned char *z, unsigned char *v)
{
	int i;
    for (i = 0; i < 16; i++) {
        z[i] = z[i] ^ v[i];
    }
}

void gf_mult(const unsigned char *x, const unsigned char *y, unsigned char *z)
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
//#pragma HLS unroll region
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
