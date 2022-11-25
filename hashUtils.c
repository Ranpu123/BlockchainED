#include <stdio.h>
#include <openssl/sha.h>

#include "hashUtils.h"

void cpyhash(unsigned char * dst, unsigned char * org){
    for(int i = 0; i<SHA256_DIGEST_LENGTH; i++){
        dst[i]=org[i];
    }
}

void printhash(unsigned char * hash){
    for(int i = 0; i<SHA256_DIGEST_LENGTH; i++){
        printf("%02x", hash[i]);
    }
    printf("\n");
}