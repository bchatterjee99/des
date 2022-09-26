#include <stdio.h>
#include <stdlib.h>

#include "des.h"
#include "ofb.h"



void ofb_enc(unsigned char* plaintext, int len, unsigned char* key, unsigned char* iv, unsigned char* ciphertext)
{
    unsigned char in[8];
    unsigned char out[8];
    for(int i=0; i<8; i++) in[i] = iv[i];

    int idx = 0;
    for(int i=0; i<len/8; i++)
    {
	des_enc(in, key, out);
	for(int j=0; j<8; j++)
	{
	    ciphertext[idx] = out[j] ^ plaintext[idx];
	    idx++;
	    in[j] = out[j]; // next input
	}
    }
}

void ofb_dec(unsigned char* ciphertext, int len, unsigned char* key, unsigned char* iv, unsigned char* decryptedtext)
{
    unsigned char in[8];
    unsigned char out[8];
    for(int i=0; i<8; i++) in[i] = iv[i];

    int idx = 0;
    for(int i=0; i<len/8; i++)
    {
	des_enc(in, key, out);
	for(int j=0; j<8; j++)
	{
	    decryptedtext[idx] = out[j] ^ ciphertext[idx];
	    idx++;
	    in[j] = out[j]; // next input
	}
    }
}
