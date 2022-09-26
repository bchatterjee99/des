#include <stdio.h>
#include "des.h"
#include "tables.h"

int get_bit(unsigned char* arr, int idx)
{
    int x = idx/8;
    int y = idx%8;
    int ans = arr[x] & (1 << (7 - y));
    ans = !!(ans);
    return ans;
}
void set_bit(unsigned char* arr, int idx, int bit)
{
    int x = idx/8;
    int y = idx%8;
    arr[x] = arr[x] | (bit << (7 - y));
}
void zero(unsigned char* arr)
{
    for(int i=0; i<8; i++) arr[i] = 0;
}


// initial permutation
void initial_permutation(unsigned char* block)
{
    unsigned char tmp_block[8];
    zero(tmp_block);

    for(int i=0; i<64; i++)
	set_bit(tmp_block, i, get_bit(block, IP[i]-1));
        // tmp_block[i] = block[ IP[i]-1 ]

    for(int i=0; i<8; i++)
	block[i] = tmp_block[i];
}

// final permutation
void final_permutation(unsigned char* block)
{
    unsigned char tmp_block[8];
    zero(tmp_block);

    for(int i=0; i<64; i++)
	set_bit(tmp_block, i, get_bit(block, IP_inv[i]-1));
        // tmp_block[i] = block[ IP[i]-1 ]

    for(int i=0; i<8; i++)
	block[i] = tmp_block[i];
}


// encrypt 64 bits starting at address block
void des(unsigned char* block)
{
    printf("\n\nDES:-------\n\n");

    
    printf("block:\n");
    for(int i=0; i<8; i++)
	printf("%2x:%c ", block[i]);
    printf("\n\n");

    initial_permutation(block);

    printf("after initial permutation:\n");
    for(int i=0; i<8; i++)
	printf("%2x:%c ", block[i]);

    printf("after final permutation:\n");
    for(int i=0; i<8; i++)
	printf("%2x:%c ", block[i]);
}
