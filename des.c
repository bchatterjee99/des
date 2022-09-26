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
void zero(unsigned char* arr, int bytes)
{
    for(int i=0; i<bytes; i++) arr[i] = 0;
}


// initial permutation
void initial_permutation(unsigned char* block)
{
    unsigned char tmp_block[8];
    zero(tmp_block, 8);

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
    zero(tmp_block, 8);

    for(int i=0; i<64; i++)
	set_bit(tmp_block, i, get_bit(block, IP_inv[i]-1));
        // tmp_block[i] = block[ IP[i]-1 ]

    for(int i=0; i<8; i++)
	block[i] = tmp_block[i];
}

void test2()
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
}


int s_box_lookup(unsigned char* block, int idx)
{
    int i = idx*6; // bit number
    int row, col; 

    row = (get_bit(block, i) << 1) | get_bit(block, i+5);

    col = get_bit(block, i+1);
    col = (col << 1) | get_bit(block, i+2);
    col = (col << 1) | get_bit(block, i+3);
    col = (col << 1) | get_bit(block, i+4);
}

// 1 round feistel structure
void feistel(unsigned char* l, unsigned char* r, int round)
{
    unsigned char r_expanded[6];
    unsigned char r_xored[6];
    zero(r_expanded, 6);
    zero(r_xored, 6);

    for(int i=0; i<48; i++)
	set_bit(r_expanded, i, get_bit(r, EXPANSION[i] - 1));
        // r_expanded[i] = r[EXPANSION[i] - 1]

    // get round_key
    // xor_48(r_expanded, round_key, r_xored);
    for(int i=0; i<6; i++) r_xored[i] = r_expanded[i];

    for(int i=0; i<8; i++)
    {
	int x = s_box_lookup(r_xored, i);
    }

}


// encrypt 64 bits starting at address block
void des_enc(unsigned char* block)
{
    test2(); // debug

    printf("\n\nDES:-------\n\n");

    
    printf("block:\n");
    for(int i=0; i<8; i++)
	printf("%2x:%c ", block[i], block[i]);
    printf("\n\n");

    initial_permutation(block);

    printf("after initial permutation:\n");
    for(int i=0; i<8; i++)
	printf("%2x:%c ", block[i], block[i]);
    printf("\n\n");




    final_permutation(block);

    printf("after final permutation:\n");
    for(int i=0; i<8; i++)
	printf("%2x:%c ", block[i], block[i]);
    printf("\n\n");
}

