#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "aes.h"

static uint8_t *read_bytes_as_hex(FILE *fp, int n){
    uint8_t *bytes = malloc(sizeof(uint8_t)*n);
    for(int i=0; i<n; i++){
        char buffer[2];

        char c = fgetc(fp);
        while(c == 32 || c == 10){
            c = fgetc(fp);
        }
        buffer[0]=c;
        buffer[1]=fgetc(fp);

        uint8_t byte = (uint8_t)strtol(buffer, NULL, 16);
        bytes[i]=byte;
    }

    return bytes;
}

uint8_t *read_sbox(FILE *fp){
    uint8_t *sBox = read_bytes_as_hex(fp, 256);
    return sBox;
}

uint8_t *read_msg(FILE *fp){
   uint8_t *msg = read_bytes_as_hex(fp, 16);
   return msg;
}

uint8_t *read_key(FILE *fp){
    uint8_t *key = read_bytes_as_hex(fp, 16);
    return key;
}


int main(int argc, char** argv){

    char *plaintextFile = argv[1];
    char *keyFile = argv[2];

    FILE *fpMsg = fopen(plaintextFile, "r");
    uint8_t *msg = read_msg(fpMsg);
    FILE *fpKey = fopen(keyFile, "r");
    uint8_t *key = read_key(fpKey);

    printf("PlainText Filename:%s\n", plaintextFile);
    printf("Key Filename:%s\n\n", keyFile);

    fclose(fpMsg);
    fclose(fpKey);

    uint8_t *cipher = aes_encrypt_128(msg, key);
    uint8_t *plainText = aes_decrypt_128(cipher, key);

    free(msg);
    free(key);
    return 0;
}

