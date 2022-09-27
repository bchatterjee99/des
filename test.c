#include <stdio.h>
#include <stdlib.h>

#include "des.h"


int main()
{
    unsigned char* plaintext = malloc(8 * sizeof(unsigned char));
    unsigned char* ciphertext = malloc(8 * sizeof(unsigned char));
    unsigned char* decryptedtext = malloc(8 * sizeof(unsigned char));
    unsigned char* key = malloc(8 * sizeof(unsigned char));

    char message[] = "thikache";
    char key_str[] = "abcdefgh";
    char iv_str[] = "ijklmnop";

    for(int i=0; i<8; i++) plaintext[i] = message[i];
    for(int i=0; i<8; i++) key[i] = key_str[i];

    printf("plaintext:\n");
    for(int i=0; i<8; i++)
	printf("%c", plaintext[i]);
    printf("\n\n");

    des_enc(plaintext, key, ciphertext);

    printf("ciphertext:\n");
    for(int i=0; i<8; i++)
	printf("%02x ", ciphertext[i]);
    printf("\n\n");

    des_dec(ciphertext, key, decryptedtext);

    printf("decryptedtext:\n");
    for(int i=0; i<8; i++)
	printf("%c", decryptedtext[i]);
    printf("\n\n");

    return 0;
}
