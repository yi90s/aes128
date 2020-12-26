#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "aes.h"

void shift_rows(uint8_t *st){
    int i = 4;
    //shift the row 1 to the left by 1 position
    uint8_t temp = st[i];
    st[i] = st[i+1];
    st[i+1] = st[i+2];
    st[i+2] = st[i+3];
    st[i+3] = temp;

    //shift the row 2 to the left by 2 position
    i=8;
    temp = st[i];
    st[i] = st[i+2];
    st[i+2] = temp;
    temp = st[i+1];
    st[i+1] = st[i+3];
    st[i+3] = temp;

    //shift the row 3 to the left by 3 position
    i=12;
    temp = st[i+3];
    st[i+3] = st[i+2];
    st[i+2] = st[i+1];
    st[i+1] = st[i];
    st[i] = temp;
}

void shift_rows_inv(uint8_t *st){
    int i = 4;
    //shift the row 1 to the right by 1 position
    uint8_t temp = st[i+3];
    st[i+3] = st[i+2];
    st[i+2] = st[i+1];
    st[i+1] = st[i];
    st[i] = temp;

    //shift the row 2 to the left by 2 position
    i=8;
    temp = st[i+3];
    st[i+3] = st[i+1];
    st[i+1] = temp;
    temp = st[i+2];
    st[i+2] = st[i];
    st[i] = temp;

    //shift the row 3 to the left by 3 position
    i=12;
    temp = st[i];
    st[i] = st[i+1];
    st[i+1] = st[i+2];
    st[i+2] = st[i+3];
    st[i+3] = temp;
}


void byte_subst(uint8_t *st){
    for(int i=0; i<16; i++){
        st[i] = sbox_subst(st[i], sbox);
    }
}

void byte_subst_inv(uint8_t *st){
    for(int i=0; i<16; i++){
        st[i] = sbox_subst(st[i], sboxInv);
    }
}

uint8_t x_time(uint8_t x){
    return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

void mix_col(uint8_t *st){
    uint8_t newState[16];

    newState[0] = mul2[st[0]]^mul3[st[4]]^st[8]^st[12];
    newState[1] = mul2[st[1]]^mul3[st[5]]^st[9]^st[13];
    newState[2] = mul2[st[2]]^mul3[st[6]]^st[10]^st[14];
    newState[3] = mul2[st[3]]^mul3[st[7]]^st[11]^st[15];

    newState[4] = st[0]^mul2[st[4]]^mul3[st[8]]^st[12];
    newState[5] = st[1]^mul2[st[5]]^mul3[st[9]]^st[13];
    newState[6] = st[2]^mul2[st[6]]^mul3[st[10]]^st[14];
    newState[7] = st[3]^mul2[st[7]]^mul3[st[11]]^st[15];

    newState[8] = st[0]^st[4]^mul2[st[8]]^mul3[st[12]];
    newState[9] = st[1]^st[5]^mul2[st[9]]^mul3[st[13]];
    newState[10] = st[2]^st[6]^mul2[st[10]]^mul3[st[14]];
    newState[11] = st[3]^st[7]^mul2[st[11]]^mul3[st[15]];

    newState[12] = mul3[st[0]]^st[4]^st[8]^mul2[st[12]];
    newState[13] = mul3[st[1]]^st[5]^st[9]^mul2[st[13]];
    newState[14] = mul3[st[2]]^st[6]^st[10]^mul2[st[14]];
    newState[15] = mul3[st[3]]^st[7]^st[11]^mul2[st[15]];


    for(int i=0 ;i<16;i++){
        st[i] = newState[i];
    }

}

void mix_col_inv(uint8_t *st){
    uint8_t newState[16];

    newState[0] = mul14[st[0]]^mul11[st[4]]^mul13[st[8]]^mul9[st[12]];
    newState[1] = mul14[st[1]]^mul11[st[5]]^mul11[st[9]]^mul13[st[13]];
    newState[2] = mul14[st[2]]^mul11[st[6]]^mul13[st[10]]^mul9[st[14]];
    newState[3] = mul14[st[3]]^mul11[st[7]]^mul13[st[11]]^mul9[st[15]];

    newState[4] = mul9[st[0]]^mul14[st[4]]^mul11[st[8]]^mul13[st[12]];
    newState[5] = mul9[st[1]]^mul14[st[5]]^mul11[st[9]]^mul13[st[13]];
    newState[6] = mul9[st[2]]^mul14[st[6]]^mul11[st[10]]^mul13[st[14]];
    newState[7] = mul9[st[3]]^mul14[st[7]]^mul11[st[11]]^mul13[st[15]];

    newState[8] = mul13[st[0]]^mul9[st[4]]^mul14[st[8]]^mul11[st[12]];
    newState[9] = mul13[st[1]]^mul9[st[5]]^mul14[st[9]]^mul11[st[13]];
    newState[10] = mul13[st[2]]^mul9[st[6]]^mul14[st[10]]^mul11[st[14]];
    newState[11] = mul13[st[3]]^mul9[st[7]]^mul14[st[11]]^mul11[st[15]];

    newState[12] = mul11[st[0]]^mul13[st[4]]^mul9[st[8]]^mul14[st[12]];
    newState[13] = mul11[st[1]]^mul13[st[5]]^mul9[st[9]]^mul14[st[13]];
    newState[14] = mul11[st[2]]^mul13[st[6]]^mul9[st[10]]^mul14[st[14]];
    newState[15] = mul11[st[3]]^mul13[st[7]]^mul9[st[11]]^mul14[st[15]];


    for(int i=0 ;i<16;i++){
        st[i] = newState[i];
    }

}

void add_round_key(uint8_t *st, const uint8_t *roundKey){
    for(int i=0; i<16; i++){
        st[i]=st[i]^roundKey[i];
    }
}

//return an array that contain the key schedule generated from the seed key
void key_expansion(uint8_t *roundKey, int round){

    uint8_t newKey[16];

    //rotate the last col and subst bytes with round constant
    uint8_t rotatedLastCol[4];
    for(int row=0;row<4; row++){

        rotatedLastCol[row]=sbox_subst(roundKey[(row+1)%4*4+3], sbox);

    };


    for(int col=0; col<4; col++){

        for(int row=0; row<4; row++){

            if(col==0){
                uint8_t rc = row==0 ? RC[round] : 0;
                newKey[row*4+col] = roundKey[row*4+col]^rotatedLastCol[row]^rc;

            }else{
                newKey[row*4+col] = roundKey[row*4+col]^newKey[row*4+col-1];
            }

        }

    }

    memcpy(roundKey, newKey, 16);
    roundKeys[round] = malloc(sizeof(uint8_t)*16);
    memcpy(roundKeys[round], newKey, 16);
}



void print_matrix_hex(const uint8_t *vec, int n){
    printf("\n");

    for(int row=0; row<n; row++){
        printf("\n");
        for(int col=0; col<n; col++){
            printf("%x ", vec[row*4+col]);
        }
    }
    printf("\n");
}

void print_vec_hex(uint8_t *vec, int n){
    for(int i=0; i<n; i++){
        printf("%x ", vec[i]);
    }
}

uint8_t *aes_encrypt_128(const uint8_t* plainText, const uint8_t* initKey){
    uint8_t *cipher = malloc(sizeof(uint8_t)*16);
    uint8_t *state;
    uint8_t *roundKey;


    state = matrix_transpose(plainText, 4);
    roundKey = matrix_transpose(initKey, 4);

    printf("\n\nENCRYPTION PROCESS:");
    printf("\n-----------------------");
    printf("\nPlain Text:\n");
    print_vec_hex(normalize_state(state), 16);

    //initial round
    add_round_key(state, roundKey);
    printf("\n\nRound 1");
    printf("\n----------\n");
    print_vec_hex(normalize_state(state), 16);

    //10 main rounds
    for(int r = 0; r < ROUND_NUM; r++){

        //generate a new key for this round
        key_expansion(roundKey, r);

        //byte substitution
        byte_subst(state);

        //shifts rows
        shift_rows(state);


        //skip the mix_column for the last encryption round
        if(r != 9){
            mix_col(state);

        }

        add_round_key(state, roundKey);
        printf("\n\nRound %d", r+2);
        printf("\n----------\n");
        print_vec_hex(normalize_state(state), 16);
    }

    printf("\n\n\nKey Schedule:\n");
    for(int i=0;i<16;i++){
        printf("%x ", initKey[i]);
    }
    for(int i=0; i<10; i++){
        printf("\n");
        print_vec_hex(normalize_state((uint8_t *)roundKeys[i]), 16);
    }


    memcpy(cipher, state, 16);

    return cipher;
};

uint8_t *aes_decrypt_128(const uint8_t* cipher, const uint8_t* initKey){
    uint8_t state[16];
    memcpy(state, cipher, 16);
    uint8_t *roundKey = (uint8_t *)roundKeys[9];

    printf("\n\n\nDECRYPTION PROCESS:");
    printf("\n-----------------------");
    printf("\nCipher Text:\n");
    print_vec_hex(normalize_state(cipher), 16);

    //undo the round 10
    add_round_key(state, roundKey);
    shift_rows_inv(state);
    byte_subst_inv(state);

    printf("\n\nRound 10");
    printf("\n----------\n");
    print_vec_hex(normalize_state(state), 16);
    //undo the remaining 9 rounds
    for(int r=8; r>=0; r--){
        add_round_key(state, (uint8_t *)roundKeys[r]);
        mix_col_inv(state);
        shift_rows_inv(state);
        byte_subst_inv(state);
        printf("\n\nRound %d", r);
        printf("\n----------\n");
        print_vec_hex(normalize_state(state),16);
    }

    add_round_key(state, initKey);
    printf("\nPlaintext:");
    printf("\n----------\n");
    print_vec_hex(normalize_state(state), 16);
    return normalize_state(state);
};

//convert plaintext string into the 4x4 matrix format for AES
uint8_t *matrix_transpose(const uint8_t *plaintext, int n){
    uint8_t *state = malloc(sizeof(uint8_t)*n*n);

    for(int row=0; row<n; row++){

        for(int col=0; col<n; col++){
            state[col*n+row] = plaintext[row*n+col]; 
        };
    };

    return state;
};

static uint8_t *normalize_state(const uint8_t *state){
    uint8_t *cipherString = malloc(sizeof(uint8_t)*16);

    for(int col=0; col<4; col++){
        for(int row=0; row<4; row++){
            cipherString[col*4+row] = state[row*4+col];
        };
    };

    return cipherString;
};

static uint8_t sbox_subst(uint8_t byte, const uint8_t *table){
    return table[(byte >> 4)*16+(byte & 0b00001111)];
};

