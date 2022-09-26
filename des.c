#include <stdio.h>
#include <stdlib.h>
#include "des.h"
#include "tables.h"


unsigned char round_keys[16][6];

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
void show_sub_block(unsigned char* arr, int idx, int sub_block_size)
{
    int i = idx * sub_block_size;
    int tmp = sub_block_size;
    while(tmp--)
    {
	printf("%d", get_bit(arr, i));
	i++;
    }
}
// put sub_block_size bit sub_block at idx'th position in arr
void put_sub_block(unsigned char* arr, int idx, int sub_block_size, int value)
{
    int x = (1<<(sub_block_size-1));
    int i = idx * sub_block_size; // bit number
    while(x > 0)
    {
	set_bit(arr, i, !!(value & x));
	x = x >> 1;
	i++;
    }
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



//void sbox_selector(int (*sbox)[16], int i)
void sbox_selector(int **sbox, int i)
{
    switch(i)
    {
    case 0:
	for(int i=0; i<4; i++)
	    for(int j=0; j<16; j++)
		sbox[i][j] = sbox1[i][j];
	break;
    case 1:
	for(int i=0; i<4; i++)
	    for(int j=0; j<16; j++)
		sbox[i][j] = sbox2[i][j];
	break;
    case 2:
	for(int i=0; i<4; i++)
	    for(int j=0; j<16; j++)
		sbox[i][j] = sbox3[i][j];
	break;
    case 3:
	for(int i=0; i<4; i++)
	    for(int j=0; j<16; j++)
		sbox[i][j] = sbox4[i][j];
	break;
    case 4:
	for(int i=0; i<4; i++)
	    for(int j=0; j<16; j++)
		sbox[i][j] = sbox5[i][j];
	break;
    case 5:
	for(int i=0; i<4; i++)
	    for(int j=0; j<16; j++)
		sbox[i][j] = sbox6[i][j];
	break;
    case 6:
	for(int i=0; i<4; i++)
	    for(int j=0; j<16; j++)
		sbox[i][j] = sbox7[i][j];
	break;
    case 7:
	for(int i=0; i<4; i++)
	    for(int j=0; j<16; j++)
		sbox[i][j] = sbox8[i][j];
	break;
    }
}
int sbox_lookup(unsigned char* block, int idx)
{
    int i = idx*6; // bit number
    int row, col; 

    row = (get_bit(block, i) << 1) | get_bit(block, i+5);

    col = get_bit(block, i+1);
    col = (col << 1) | get_bit(block, i+2);
    col = (col << 1) | get_bit(block, i+3);
    col = (col << 1) | get_bit(block, i+4);

    int **sbox = malloc(4 * sizeof(int*));
    for(int i=0; i<4; i++) sbox[i] = malloc(16*sizeof(int));
    
    sbox_selector(sbox, idx);

    /* printf("selected sbox:\n"); */
    /* for(int i =0; i<4; i++) */
    /* { */
    /* 	for(int j=0; j<16; j++) */
    /* 	    printf("%2d ", sbox[i][j]); */
    /* 	printf("\n"); */
    /* } */
    /* printf("\n\n"); */

    int ans = sbox[row][col];

    for(int i=0; i<4; i++) free(sbox[i]);
    free(sbox);

    return ans;
}

void f(unsigned char* in, unsigned char* out, int round)
{
    unsigned char tmp[4];

    unsigned char expanded[6];
    unsigned char xored[6];
    zero(expanded, 6);
    zero(xored, 6);
    zero(tmp, 4);
    zero(out, 4);

    for(int i=0; i<48; i++)
	set_bit(expanded, i, get_bit(in, EXPANSION[i] - 1));
        // expanded[i] = in[EXPANSION[i] - 1]

    // xor with round_key
    for(int i=0; i<6; i++) xored[i] = expanded[i] ^ round_keys[round][i];

    // printf("sbox:\n");
    for(int i=0; i<8; i++)
    {
	// sbox of ith 6 bit block
	// show_sub_block(xored, i, 6); printf(": ");
	int x = sbox_lookup(xored, i);
	put_sub_block(tmp, i, 4, x);
	// printf("%d\n", x);
    }
    // printf("\n\n");

    // permutation P
    for(int i=0; i<32; i++)
	set_bit(out, i, get_bit(tmp, P[i] - 1));
        // out[i] = tmp[P[i] - 1]
}

// 1 round feistel structure (encryption)
void feistel1(unsigned char* l, unsigned char* r, int round)
{
    // f_out
    unsigned char f_out[4];
    f(r, f_out, round);

    // save  R
    unsigned char r_back_up[4];
    for(int i=0; i<4; i++) r_back_up[i] = r[i];

    // R'
    for(int i=0; i<4; i++) r[i] = l[i] ^ f_out[i];

    // L'
    for(int i=0; i<4; i++) l[i] = r_back_up[i];
}
// 1 round feistel structure (decryption)
void feistel2(unsigned char* l, unsigned char* r, int round)
{
    // f_out
    unsigned char f_out[4];
    f(l, f_out, round);

    // save  L
    unsigned char l_back_up[4];
    for(int i=0; i<4; i++) l_back_up[i] = l[i];

    // L'
    for(int i=0; i<4; i++) l[i] = r[i] ^ f_out[i];

    // R'
    for(int i=0; i<4; i++) r[i] = l_back_up[i];
}


int get_shift(int round)
{
    if(round == 1) return 1;
    if(round == 2) return 1;
    if(round == 9) return 1;
    if(round == 16) return 1;
    return 2;
}
void key_schedule(unsigned char* key, int reversed)
{
    unsigned char curr_key[56];
    unsigned char tmp_key[56];
    zero(curr_key, 7);

    // PC-1
    for(int i=0; i<56; i++)
	set_bit(curr_key, i, get_bit(key, PC_1[i] - 1));

    for(int i=0; i<16; i++)
    {
	// shift = 1 or 2 
	int shift = get_shift(i);
	zero(tmp_key, 7);

	// rotate C
	for(int j=0; j<28; j++)
	{
	    int from = j + shift; from = from % 28;
	    set_bit(tmp_key, j, get_bit(curr_key, from));
	}

	// rotate D
	for(int j=0; j<28; j++)
	{
	    int to = j + 28;
	    int from = j + shift; from = from % 28 + 28;
	    set_bit(tmp_key, to, get_bit(curr_key, from));
	}

	// copy to curr_key
	for(int j=0; j<7; j++) curr_key[j] = tmp_key[j];

	// PC-2
	int idx = i;
	if(reversed) idx = 15 - idx;
	zero(round_keys[idx], 6);
	for(int j=0; j<48; j++)
	    set_bit(round_keys[idx], j, get_bit(curr_key, PC_2[j]));
    }

}

void test2(unsigned char* key)
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


    key_schedule(key, 0);
    printf("round keys:\n");
    for(int i=0; i<16; i++)
    {
	for(int j=0; j<6; j++)
	    printf("%02x", round_keys[i][j]);
	printf("\n");
    }
    printf("\n\n");

    key_schedule(key, 1);
    printf("round keys reversed:\n");
    for(int i=0; i<16; i++)
    {
	for(int j=0; j<6; j++)
	    printf("%02x", round_keys[i][j]);
	printf("\n");
    }
    printf("\n\n");
}


// encrypt 64 bits starting at address "block"
void des_enc(unsigned char* block, unsigned char* key, unsigned char* out)
{
    // test2(key); // debug
    // printf("\n\nDES encrypt:-------\n\n");

    key_schedule(key, 0);

    for(int i=0; i<8; i++) out[i] = block[i];
    
    /* printf("out:\n"); */
    /* for(int i=0; i<8; i++) */
    /* 	printf("%2x:%c ", out[i], out[i]); */
    /* printf("\n\n"); */

    initial_permutation(out);

    /* printf("after initial permutation:\n"); */
    /* for(int i=0; i<8; i++) */
    /* 	printf("%02x:%c ", out[i], out[i]); */
    /* printf("\n\n"); */

    for(int i=0; i<16; i++)
    {
	feistel1(out, &out[4], i);
    }

    final_permutation(out);

    /* printf("after final permutation:\n"); */
    /* for(int i=0; i<8; i++) */
    /* 	printf("%02x:%c ", out[i], out[i]); */
    /* printf("\n\n"); */
}

// decrypt 64 bits starting at address "block"
void des_dec(unsigned char* block, unsigned char* key, unsigned char* out)
{
    // test2(key); // debug

    printf("\n\nDES decrypt:-------\n\n");

    key_schedule(key, 1);

    for(int i=0; i<8; i++) out[i] = block[i];
    
    /* printf("out:\n"); */
    /* for(int i=0; i<8; i++) */
    /* 	printf("%2x:%c ", out[i], out[i]); */
    /* printf("\n\n"); */

    initial_permutation(out);

    /* printf("after initial permutation:\n"); */
    /* for(int i=0; i<8; i++) */
    /* 	printf("%02x:%c ", out[i], out[i]); */
    /* printf("\n\n"); */

    for(int i=0; i<16; i++)
    {
	feistel2(out, &out[4], i);
    }

    final_permutation(out);

    /* printf("after final permutation:\n"); */
    /* for(int i=0; i<8; i++) */
    /* 	printf("%02x:%c ", out[i], out[i]); */
    /* printf("\n\n"); */
}
