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

//Carrega o vetor com os valores de cada cardeira do bloco
void loadWallet(int * wallet){
    FILE * pFile = fopen("data/blockchain.dat","rb");
    fseek(pFile, -1024L, SEEK_END);
    fread(wallet, sizeof(int), 256, pFile);
}

//Carrega o bloco com o indice informado
BlocoMinerado searchBlock(int index){
    BlocoMinerado aux;
    aux.bloco.numero = -1;

    FILE * pFile = fopen("data/blockchain.dat","rb");
    fseek(pFile, -1024L, SEEK_END);
    int sz = ftell(pFile)/sizeof(BlocoMinerado);
    if(index > 0 && index <=sz){
        fseek(pFile, (index-1)*sizeof(BlocoMinerado), SEEK_SET);
        fread(&aux, sizeof(BlocoMinerado), 1, pFile);  
    }else{
        printf("ERRO: Indice inválido: Valor deve ser maior que 0, e menor ou igual a %d \n",sz);
    }

    return aux;
}
