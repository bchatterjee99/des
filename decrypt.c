#include <stdio.h>
#include <stdlib.h>
#include "des.h"
#include "ofb.h"

unsigned char* decryptedtext;
int padded_len;
unsigned char key[8];
unsigned char* ciphertext;
unsigned char iv[8];

void input_ciphertext()
{
    int i;
    FILE* fp = fopen("ciphertext", "r");
    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    rewind(fp);

    // iv
    for(int i=0; i<8; i++)
	fread(iv + i, 1, 1, fp);

    // first 8 bytes iv
    len -= 8;

    padded_len = len/8;
    if(len % 8) padded_len++;
    padded_len = padded_len * 8;

    decryptedtext = malloc(padded_len*sizeof(unsigned char));
    ciphertext = malloc(padded_len*sizeof(unsigned char));


    // ciphertext
    for(i=0; i<len; i++)
	fread(ciphertext + i, 1, 1, fp);

    // padding
    int p = 0;
    if(len % 8 != 0)
	p = 8 - (len % 8);
    while(p--) { ciphertext[i] = 0; i++; }

    fclose(fp);
}

void input_key()
{
    int i;
    FILE* fp = fopen("key", "r");
    fseek(fp, 0, SEEK_END);
    int key_len = ftell(fp);
    if(key_len < 8)
    {
	printf("error: key not 64 bits\n\n");
	exit(1);
    }
    rewind(fp);

    for(i=0; i<8; i++)
	fread(key + i, 1, 1, fp);

    fclose(fp);
}

void test1()
{

    printf("ciphertext:\n");
    for(int i=0; i<padded_len; i++)
	printf("0x%02x ", ciphertext[i]);
    printf("\n\n");

    des_dec(ciphertext, key, decryptedtext);

    printf("decrypted text:\n");
    for(int i=0; i<8; i++)
	printf("%c", decryptedtext[i]);
    printf("\n\n");
}

int main()
{
    input_ciphertext();
    input_key();

    printf("ciphertext:\n");
    for(int i=0; i<padded_len; i++)
	printf("%02x ", ciphertext[i]);
    printf("\n\n");


// void ofb_dec(unsigned char* ciphertext, int len, unsigned char* key, unsigned char* iv, unsigned char* decryptedtext);
    ofb_dec(ciphertext, padded_len, key, iv, decryptedtext);


    printf("decryptedtext:\n");
    for(int i=0; i<padded_len; i++)
	printf("%c", decryptedtext[i]);
    printf("\n\n");

    free(decryptedtext);
    free(ciphertext);
    return 0;
}
