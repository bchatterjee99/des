#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "des.h"
#include "ofb.h"

unsigned char* plaintext;
int padded_len;
unsigned char key[8];
unsigned char* ciphertext;

void input_plaintext()
{
    int i;
    FILE* fp = fopen("plaintext.txt", "r");
    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    rewind(fp);

    padded_len = len/8;
    if(len % 8) padded_len++;
    padded_len = padded_len * 8;

    plaintext = malloc(padded_len*sizeof(unsigned char));
    ciphertext = malloc(padded_len*sizeof(unsigned char));

    for(i=0; i<len; i++)
	fread(plaintext + i, 1, 1, fp);

    // padding
    int p = 0;
    if(len % 8 != 0)
	p = 8 - (len % 8);
    while(p--) { plaintext[i] = 0; i++; }

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

}

int main()
{
    srand(time(0));

    input_plaintext();
    input_key();

    printf("plaintext:\n");
    for(int i=0; i<padded_len; i++)
	printf("%c", plaintext[i]);
    printf("\n\n");


    unsigned char iv[8];
    for(int i=0; i<8; i++) iv[i] = rand();

    ofb_enc(plaintext, padded_len, key, iv, ciphertext);

    printf("ciphertext:\n");
    for(int i=0; i<padded_len; i++)
	printf("%02x ", ciphertext[i]);
    printf("\n\n");

    FILE* fp = fopen("ciphertext", "w");
    for(int i=0; i<8; i++)
	fputc(iv[i], fp);
    for(int i=0; i<padded_len; i++)
	fputc(ciphertext[i], fp);
    fclose(fp);


    free(plaintext);
    free(ciphertext);
    return 0;
}
