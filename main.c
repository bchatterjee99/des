#include <stdio.h>
#include "des.h"

unsigned char plaintext[8];
int len;
unsigned char key[7];
unsigned char ciphertext[8];

void input()
{
    int i;
    FILE* fp = fopen("plaintext.txt", "r");
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    rewind(fp);

    for(i=0; i<len; i++)
	fread(plaintext + i, 1, 1, fp);

    // padding
    int p = 0;
    if(len % 8 != 0)
	p = 8 - (len % 8);
    while(p--) { plaintext[i] = 0; i++; }


    fclose(fp);
}

void test1()
{
    printf("IP table:\n");
    for(int i=0; i<8; i++)
    {
	for(int j=0; j<8; j++)
	{
	    printf("%2d  ", IP[i*8 + j]);
	}
	printf("\n");
    }
    printf("\n\n");

    printf("plaintext:\n");
    for(int i=0; i<len; i++)
	printf("%c", plaintext[i], plaintext[i]);
    printf("\n\n");

    des(plaintext);
}

int main()
{
    input();

    test1();

    return 0;
}
